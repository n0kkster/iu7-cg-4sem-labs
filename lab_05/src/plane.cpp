#include "plane.h"

#include "fill.h"
#include "line.h"
#include <sys/time.h>

#include <QDebug>
#include <QMessageBox>
#include <QPaintEvent>
#include <QPen>

// Конструктор
// ==================================================
Plane::Plane(QWidget *parent) : QGraphicsView(parent) { fillEnabled = false; }

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
    QPainter painter(viewport());
    dimensions_t dim = { .xmin = viewport()->width(), .xmax = 0, .ymin = viewport()->height(), .ymax = 0 };

    unsigned long long beg, end;

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

        if (shape.need_fill)
            outline(painter, outline_points, shape, dim);
    }
    end = micros();

    if (fillEnabled)
    {
        qDebug() << "xmin:" << dim.xmin << "xmax:" << dim.xmax << "ymin:" << dim.ymin << "ymax:" << dim.ymax;
        beg = micros();
        fill(painter, outline_points, Qt::yellow, dim);
        end = micros();
        qDebug() << "time taken to fill:" << (end - beg) / 1000;
    }

    fillEnabled = false;
}

void Plane::mousePressEvent(QMouseEvent *event)
{
    switch (event->button())
    {
        case Qt::LeftButton:
            if (addVertex(event->pos()))
                emit clicked(event->pos());
            break;

        case Qt::RightButton:
            finishShapeEntering();
            emit shapeFinished(); // todo check
            break;

        default:
            break;
    }

    viewport()->update();
}

// ~================ СОБЫТИЯ ================~

bool Plane::addVertex(const QPoint &vertex)
{
    if (shapes.size() == 0)
        shapes.append(shape_t{ .need_fill = false });

    shape_t &shape = shapes.last();
    if (!appendToShape(shape.vertices, shape.edges, vertex))
    {
        QMessageBox::critical(this, "Ошибка", "Такая точка уже существует!");
        return false;
    }
    return true;
}

bool Plane::appendToShape(QVector<point_t> &vertices, QVector<edge_t> &edges, const QPoint &vertex)
{
    point_t _vertex = { vertex.x(), vertex.y() };

    for (const auto &v : vertices)
        if (v.x == _vertex.x && v.y == _vertex.y)
            return false;

    if (vertices.size() > 0)
        edges.append({ vertices.last(), _vertex });
    vertices.append(_vertex);

    return true;
}

void Plane::finishShapeEntering()
{
    if (shapes.size() == 0)
    {
        QMessageBox::critical(this, "Ошибка", "Фигура не была добавлена. Замыкать нечего!");
        return;
    }

    if (connectShape(shapes.last()) != true)
        return;

    shapes.append(shape_t{ .need_fill = false });

    viewport()->update();
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
    shapes.clear();
    outline_points.clear();
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
