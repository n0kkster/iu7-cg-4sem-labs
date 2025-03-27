#include "builtin.h"

#include <QPainter>

void drawCicrleBuiltin(QPainter &painter, const circle_t &circle, bool measure_mode = false)
{
    painter.drawEllipse(circle.center, circle.radius, circle.radius);
}

void drawEllipseBuiltin(QPainter &painter, const ellipse_t &ellipse, bool measure_mode = false)
{
    painter.drawEllipse(ellipse.center, ellipse.rx, ellipse.ry);
}