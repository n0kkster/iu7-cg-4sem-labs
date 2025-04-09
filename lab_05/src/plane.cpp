#include <QPaintEvent>
#include <QDebug>
#include <QPen>
#include <QMessageBox>

#include "plane.h"
#include "line.h"

// Конструктор
// ==================================================
Plane::Plane(QWidget *parent) : QGraphicsView(parent) 
{
}
// ==================================================

// ================= СОБЫТИЯ =================
void Plane::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(viewport());

    for (const shape_t &shape : shapes)
    {
        for (const point_t &vertex : shape.vertices)
            painter.drawPoint(vertex.x, vertex.y);

        for (const edge_t &line : shape.edges)
            drawLine(painter, line);
    }
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
            emit shapeFinished();
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
        shapes.append(shape_t{});

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
    point_t _vertex = {vertex.x(), vertex.y()};

    for (const auto &v : vertices)
        if (v.x == _vertex.x && v.y == _vertex.y)
            return false;

    if (vertices.size() > 0)
        edges.append({vertices.last(), _vertex});
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

    shapes.append(shape_t{});
    
    viewport()->update();
}

bool Plane::connectShape(shape_t &shape)
{
    if (shape.vertices.size() <= 2)
    {
        QMessageBox::critical(this, "Ошибка", "Невозможно замкнуть фигуру. Фигура должна содержать не менее трех точек!");
        return false;
    }
    
    shape.edges.append({shape.vertices.last(), shape.vertices.first()});
    return true;
}

void Plane::clearPlane()
{
    shapes.clear();
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

    painter.setPen({Qt::gray, 2});

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

