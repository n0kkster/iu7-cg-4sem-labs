#include "plane.h"

#include "circle.h"
#include "ellipse.h"
#include "fill.h"
#include "line.h"
#include <sys/time.h>

#include <QDebug>
#include <QGuiApplication>
#include <QImage>
#include <QMessageBox>
#include <QPaintEvent>
#include <QPen>
#include <QTimer>

// Конструктор
// ==================================================
Plane::Plane(QWidget *parent) : QGraphicsView(parent)
{
    fillEnabled = false;
    delayEnabled = false;
    stop_line = 0;
    fillColor = QColor::fromRgb(255, 255, 255);
    fillTime = 0;
    ready = false;

    buffer = QImage({900, 780}, QImage::Format_ARGB32);
}

// ==================================================

static unsigned long long micros(void)
{
    struct timeval value;
    gettimeofday(&value, NULL);
    return (unsigned long long)value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}

// ================= СОБЫТИЯ =================
void Plane::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(&buffer);

    viewport()->setAttribute(Qt::WA_OpaquePaintEvent, true);

    dimensions_t dim = { .xmin = viewport()->width(), .xmax = 0, .ymin = viewport()->height(), .ymax = 0 };
    unsigned long long beg, end;
    std::map<int, std::vector<point_t>> temp_outline;

    painter.setPen({Qt::white, 1});

    ready = false;

    beg = micros();
    for (shape_t &shape : shapes)
    {
        for (const point_t &vertex : shape.vertices)
            painter.drawPoint(vertex.x, vertex.y);

        for (const edge_t &line : shape.edges)
            drawLine(painter, line);

        updateDimensions(dim, shape);

        if (!shape.need_fill && fillEnabled && !shape.vertices.empty())
            shape.need_fill = true;

        if (!shape.outlined && shape.need_fill)
        {
            outline(painter, temp_outline, shape, dim);
            shape.outlined = true;
        }
    }

    for (const circle_t &circle : circles)
    {
        drawCicrle(painter, circle);
    }

    for (const ellipse_t &ellipse : ellipses)
    {
        drawEllipse(painter, ellipse);
    }

    painter.setPen({ Qt::yellow, 2 });
    for (const point_t &seed : seed_points)
    {
        painter.drawPoint(seed.x, seed.y);
    }
    painter.setPen({ Qt::white, 1 });

    if (fillEnabled)
    {
        for (const auto &[y, points] : temp_outline)
        {
            if (outline_points.contains(y))
                for (const auto &point : points)
                    outline_points[y].push_back(point);
            else
                outline_points[y] = points;
        }

        temp_outline.clear();
    }

    if (!outline_points.empty() && fillEnabled)
        fill(painter, outline_points, fillColor, dim, delayEnabled, stop_line);
    end = micros();

    if (!delayEnabled && fillEnabled)
    {
        fillTime = (end - beg) / 1000;
        ready = true;
    }
    else
    {
        fillTime = 0;
        ready = false;
    }

    if (!delayEnabled)
        fillEnabled = false;

    QPainter real_painter(this->viewport());
    real_painter.drawImage(0, 0, buffer);

    // qDebug() << "color at (0,0):" << this->grab().toImage().pixelColor(0, 0);
}

void Plane::mousePressEvent(QMouseEvent *event)
{
    Qt::KeyboardModifiers m;
    bool Z = false;
    switch (event->button())
    {
        case Qt::LeftButton:
            m = QGuiApplication::keyboardModifiers();
            if (m.testFlag(Qt::ControlModifier))
                Z = true;

            if (addVertex(event->pos(), Z))
                emit clicked(event->pos());

            break;

        case Qt::RightButton:
            if (finishShapeEntering())
                emit shapeFinished();
            break;

        case Qt::MiddleButton:
            addSeed(event->pos());
            break;

        default:
            break;
    }

    viewport()->update();
}

void Plane::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;

    if (addVertex(event->pos()))
        emit clicked(event->pos());

    viewport()->update();
}

// ~================ СОБЫТИЯ ================~

void Plane::resetShapes()
{
    for (auto &shape : shapes)
    {
        shape.need_fill = false;
        shape.outlined = false;
    }
}

void Plane::fillSlowed()
{
    stop_line = 0;
    dimensions_t dim = { .xmin = viewport()->width(), .xmax = 0, .ymin = viewport()->height(), .ymax = 0 };
    for (shape_t &shape : shapes)
        updateDimensions(dim, shape);

    int max_lines = dim.ymax - dim.ymin;

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this,
            [=, this]()
            {
                if (stop_line < max_lines)
                {
                    stop_line++;
                    viewport()->update();
                }
                else
                {
                    timer->stop();
                    timer->deleteLater();
                }
            });
    timer->start(10);

    outline_points.clear();

    resetShapes();
}

void Plane::addCircle(const circle_t &circle) { circles.append(circle); }

void Plane::addEllipse(const ellipse_t &ellipse) { ellipses.append(ellipse); }

void Plane::addSeed(const QPoint &pos) { seed_points.append({ pos.x(), pos.y() }); }

bool Plane::addVertex(const QPoint &vertex, bool Z)
{
    if (shapes.size() == 0)
        shapes.append(shape_t{ .need_fill = false, .outlined = false });

    shape_t &shape = shapes.last();
    if (!appendToShape(shape.vertices, shape.edges, vertex, Z))
    {
        QMessageBox::critical(this, "Ошибка", "Такая точка уже существует!");
        return false;
    }
    return true;
}

bool Plane::appendToShape(QVector<point_t> &vertices, QVector<edge_t> &edges, const QPoint &vertex, bool Z)
{
    point_t _vertex = { vertex.x(), vertex.y() };

    for (const auto &v : vertices)
        if (v.x == _vertex.x && v.y == _vertex.y)
            return false;

    if (Z && !vertices.empty())
    {
        int dx, dy;
        int lx = vertices.last().x, ly = vertices.last().y;

        dx = std::abs(_vertex.x - lx);
        dy = std::abs(_vertex.y - ly);

        if (dx < dy)
            _vertex.x = lx;
        else
            _vertex.y = ly;
    }

    if (vertices.size() > 0)
        edges.append({ vertices.last(), _vertex });
    vertices.append(_vertex);

    return true;
}

bool Plane::finishShapeEntering()
{
    if (shapes.size() == 0)
    {
        QMessageBox::critical(this, "Ошибка", "Фигура не была добавлена. Замыкать нечего!");
        return false;
    }

    if (connectShape(shapes.last()) != true)
        return false;

    shapes.append(shape_t{ .need_fill = false, .outlined = false });

    viewport()->update();

    return true;
}

bool Plane::connectShape(shape_t &shape)
{
    if (shape.vertices.size() <= 2)
    {
        QMessageBox::critical(this, "Ошибка",
                              "Невозможно замкнуть фигуру. Фигура должна "
                              "содержать не менее трех точек!");
        return false;
    }

    shape.edges.append({ shape.vertices.last(), shape.vertices.first() });
    return true;
}

void Plane::clearPlane()
{
    buffer.fill({39, 40, 41});

    shapes.clear();
    circles.clear();
    ellipses.clear();

    outline_points.clear();

    if (delayEnabled)
        viewport()->setAttribute(Qt::WA_OpaquePaintEvent, false);

    viewport()->update();
}

int Plane::getTotalPointsCount() const
{
    int total = 0;
    for (const auto &shape : shapes)
        total += shape.vertices.size();
    return total;
}

// ================= РИСОВАЛКИ =================
void Plane::drawAxis(QPainter &painter)
{
    const int w = viewport()->width();
    const int h = viewport()->height();

    const int span = 50;

    painter.setPen({ Qt::gray, 2 });

    painter.drawLine(0, 0, w, 0);
    painter.drawLine(0, 0, 0, h);

    for (int i = span; i <= w; i += span)
    {
        painter.drawLine(i, 0, i, 10);
        painter.drawText(i - 23, 16, QString().number(i));
    }

    for (int i = span; i <= h; i += span)
    {
        painter.drawLine(0, i, 10, i);
        painter.drawText(5, i - 10, QString().number(i));
    }
}

void Plane::drawGrid(QPainter &painter)
{
    QPen line_pen(Qt::gray, 1);
    painter.setPen(line_pen);

    const int w = viewport()->width();
    const int h = viewport()->height();

    const int span = 50;

    for (int x = span; x < w; x += span)
        drawDashedVLine(painter, x, 0, h, 5, 10);

    for (int y = span; y < h; y += span)
        drawDashedHLine(painter, y, 0, w, 5, 10);
}

void Plane::drawDashedVLine(QPainter &painter, int x, int y1, int y2, int gap, int dash_len)
{
    for (; y1 < y2; y1 += dash_len + gap)
        painter.drawLine(x, y1, x, y1 + dash_len);
}

void Plane::drawDashedHLine(QPainter &painter, int y, int x1, int x2, int gap, int dash_len)
{
    for (; x1 < x2; x1 += dash_len + gap)
        painter.drawLine(x1, y, x1 + dash_len, y);
}

// ~================ РИСОВАЛКИ ================~
