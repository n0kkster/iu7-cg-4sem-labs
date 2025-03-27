#include "builtin.h"

#include <QPainter>

void drawCircleBuiltin(QPainter &painter, const circle_t &circle, bool measure_mode)
{
    (void)measure_mode;
    if (!measure_mode)
        painter.drawEllipse(circle.center, circle.radius, circle.radius);
}

void drawEllipseBuiltin(QPainter &painter, const ellipse_t &ellipse, bool measure_mode)
{
    (void)measure_mode;
    if (!measure_mode)
        painter.drawEllipse(ellipse.center, ellipse.rx, ellipse.ry);
}