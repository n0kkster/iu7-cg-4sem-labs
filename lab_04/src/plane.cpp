#include <QPaintEvent>
#include <QDebug>
#include <QPen>
#include <QPalette>

#include "plane.h"

#include "canonical.h"
#include "parametric.h"
#include "midpoint.h"
#include "bresenham.h"

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
    // drawGrid(painter);

    for (const auto& circle : circles)
        myDrawCircle(painter, circle);

    for (const auto& ellipse : ellipses)
        myDrawEllipse(painter, ellipse);
}

void Plane::myDrawCircle(QPainter &painter, const circle_t &circle)
{
    painter.setPen(circle.color);
    switch (circle.method)
    {
        case BUILTIN:
            painter.drawEllipse(circle.center, circle.radius, circle.radius);
            break;

        case CANONICAL:
            drawCicrleCanonical(painter, circle);
            break;
        
        case PARAMETRIC:
            drawCicrleParametric(painter, circle);
            break;
        
        case BRES:
            break;
        
        case MIDPOINT:
            drawCicrleMidpoint(painter, circle);
            break;
        
        default:
            break;
    }
}

void Plane::myDrawEllipse(QPainter &painter, const ellipse_t &ellipse)
{
    painter.setPen(ellipse.color);
    switch (ellipse.method)
    {
        case BUILTIN:
            painter.drawEllipse(ellipse.center, ellipse.rx, ellipse.ry);
            break;

        case CANONICAL:
            drawEllipseCanonical(painter, ellipse);
            break;
        
        case PARAMETRIC:
            drawEllipseParametric(painter, ellipse);
            break;
        
        case BRES:
            break;
        
        case MIDPOINT:
            drawEllipseMidpoint(painter, ellipse);
            break;
        
        default:
            break;
    }
}

void Plane::addCircle(circle_t circle)
{
    circles.append(circle);
}

void Plane::addEllipse(ellipse_t ellipse)
{
    ellipses.append(ellipse);
}

void Plane::addSpectreCircle(circle_t circle, double step, int cnt)
{
    for (int i = 0; i < cnt; i++)
    {
        circles.append(circle);
        circle.radius += step;
    }
}

void Plane::addSpectreEllipse(ellipse_t ellipse, double step, int cnt)
{
    for (int i = 0; i < cnt; i++)
    {
        ellipses.append(ellipse);
        ellipse.rx += step;
        ellipse.ry += step;
    }
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
    circles.clear();
    ellipses.clear();

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