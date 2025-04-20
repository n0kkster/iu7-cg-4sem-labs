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
    iter_stop = INT_MAX;
    iter_max = 100;
    fillColor = QColor::fromRgb(255, 255, 255);
    fillTime = 0;
    ready = false;

    buffer = QImage({ 900, 780 }, QImage::Format_ARGB32);
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
    if (delayEnabled)
        buffer.fill({ 39, 40, 41 });

    QGraphicsView::paintEvent(event);
    QPainter painter(&buffer);
    QPainter real_painter(this->viewport());

    unsigned long long beg, end;

    painter.setPen({ Qt::white, 1 });

    ready = false;

    beg = micros();
    for (shape_t &shape : shapes)
    {
        for (const point_t &vertex : shape.vertices)
            painter.drawPoint(vertex.x, vertex.y);

        for (const edge_t &line : shape.edges)
            drawLine(painter, line);
    }

    for (const circle_t &circle : circles)
        drawCicrle(painter, circle);

    for (const ellipse_t &ellipse : ellipses)
        drawEllipse(painter, ellipse);

    painter.setPen({ Qt::yellow, 2 });
    for (const point_t &seed : seed_points)
        painter.drawPoint(seed.x, seed.y);
    painter.setPen({ Qt::white, 1 });

    if (fillEnabled)
    {
        painter.setPen({ fillColor, 1 });
        iter_max = 0;
        for (const point_t &seed : seed_points)
            iter_max = std::max(
                fill(painter, buffer, seed, QColor::fromRgb(255, 255, 255), fillColor, iter_stop), iter_max);
        // qDebug() << "iter max:" << iter_max;
        painter.setPen({ Qt::white, 1 });
    }

    real_painter.drawImage(0, 0, buffer);

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
    iter_stop = 0;
    qDebug() << "slowed filin, iter max:" << iter_max;
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this,
            [=, this]()
            {
                if (iter_stop < iter_max)
                {
                    // qDebug() << "inside of timer, stop:" << iter_stop << "max:" << iter_max;
                    iter_stop += 100;
                    viewport()->update();
                }
                else
                {
                    qDebug() << "timer stop";
                    iter_stop = INT_MAX;
                    fillEnabled = false;
                    timer->stop();
                    timer->deleteLater();
                }
            });
    timer->start(10);
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
    buffer.fill({ 39, 40, 41 });

    shapes.clear();
    circles.clear();
    ellipses.clear();
    seed_points.clear();

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
