#include <QPaintEvent>
#include <QDebug>
#include <QPen>
#include <QPalette>

#include "plane.h"

#include "dda.h"
#include "bresenham.h"
#include "wu.h"

// Конструктор
// ==================================================
Plane::Plane(QWidget *parent) : QGraphicsView(parent) 
{
    color.setRgb(39, 40, 41);
}
// ==================================================

void Plane::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(viewport());

    drawAxis(painter);
    drawGrid(painter);

    for (const auto& line : lines)
        myDrawLine(painter, line);
}

void Plane::myDrawLine(QPainter &painter, const line_t &line)
{
    painter.setPen(line.color);
    switch (line.method)
    {
        case BUILTIN:
            painter.drawLine(line.start, line.end);
            break;

        case DDA:
            dda(painter, line);
            break;
        
        case BRES_REAL:
            bres_real(painter, line);
            break;
        
        case BRES_INT:
            bres_int(painter, line);
            break;
        
        case BRES_SMOOTH:
            bres_smooth(painter, line);
            break;

        case WU:
            wu(painter, line);
            break;
        
        default:
            break;
    }
}

void Plane::addLine(line_t line)
{
    lines.append(line);
}

void Plane::addSpectre(double length, method_e method, double angle, const QColor &color)
{
    int steps = 360 / angle;
    double cx = viewport()->width() / 2;
    double cy = viewport()->height() / 2;

    angle = qDegreesToRadians(angle);
    line_t line = {{cx, cy}, {cx + length, cy}, method, color};
    for (int i = 0; i < steps; i++)
    {
        addLine(line);
        rotatePoint(angle, line.end, line.start);
    }
}

void Plane::rotatePoint(double angle, QPointF &point, const QPointF &origin)
{
    double nx, ny;
    double cx = origin.x(), cy = origin.y();
    double x = point.x(), y = point.y();

    nx = cx + (x - cx) * cos(angle) + (y - cy) * sin(angle);
    ny = cy - (x - cx) * sin(angle) + (y - cy) * cos(angle);
    
    point.setX(nx);
    point.setY(ny);
}

void Plane::setBGColor(const QColor &color)
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Base, color);
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    this->viewport()->update();
}

void Plane::clearPlane()
{
    lines.clear();
    viewport()->update();
}

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