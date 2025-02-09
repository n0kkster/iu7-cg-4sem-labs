#include "plane.h"

Plane::Plane(QWidget *parent) : QGraphicsView(parent) 
{
    pointsCount = 0;
}

void Plane::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(viewport());
    QPen dots_pen(Qt::yellow, 2);

    drawGrid(painter, 50);
    drawAxis(painter);

    painter.setPen(dots_pen);
    for (const auto &point : points)
        painter.drawPoint(realCoordToScreenCoord(point.second));

    drawTriangle(painter);
}

void Plane::addTriangle(std::array<QPointF, 3> trianglePoints)
{
    triangle = trianglePoints;
    update();
}

void Plane::drawTriangle(QPainter &painter)
{
    QPen triangle_pen(Qt::green, 2);
    
    if (triangle.empty())
        return;

    painter.setPen(triangle_pen);

    for (const auto &point : triangle)
        painter.drawPoint(realCoordToScreenCoord(point));

    painter.drawLine(realCoordToScreenCoord(triangle[0]), realCoordToScreenCoord(triangle[1]));
    painter.drawLine(realCoordToScreenCoord(triangle[1]), realCoordToScreenCoord(triangle[2]));
    painter.drawLine(realCoordToScreenCoord(triangle[2]), realCoordToScreenCoord(triangle[0]));
}

void Plane::mousePressEvent(QMouseEvent *event) 
{
    QPointF point = screenCoordToRealCoord(mapToScene(event->pos()));
    points.append(std::pair<int, QPointF>{++pointsCount, point});

    emit clicked(point);

    this->viewport()->repaint();
}

void Plane::drawGrid(QPainter &painter, int span)
{
    QPen line_pen(Qt::gray, 1);
    painter.setPen(line_pen);

    for (int x = span - 1; x < this->viewport()->size().width(); x += span)
        drawDashedVLine(painter, x, 0, this->viewport()->size().height(), 5, 10);
    for (int y = span - 1; y < this->viewport()->size().height(); y += span)
        drawDashedHLine(painter, y, 0, this->viewport()->size().width(), 5, 10);
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

void Plane::drawAxis(QPainter &painter)
{
    QPen line_pen(Qt::gray, 3);
    painter.setPen(line_pen);

    const int line_width = 400;
    int center_x = this->viewport()->size().width() / 2;
    int center_y = this->viewport()->size().height() / 2;

    painter.drawLine(center_x - line_width / 2, center_y, center_x + line_width / 2, center_y);
    painter.drawLine(center_x, center_y - line_width / 2, center_x, center_y + line_width / 2);
}

QPointF Plane::screenCoordToRealCoord(QPointF point)
{
    return QPointF{point.rx(), -point.ry()};
}

QPointF Plane::realCoordToScreenCoord(QPointF point)
{
    return QPointF{point.rx() + this->viewport()->size().width() / 2, -point.ry() + this->viewport()->size().height() / 2};
}
