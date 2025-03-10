#include "dda.h"

void dda(QPainter &painter, const line_t &line)
{
    double dx, dy, len;
    double x, y;

    dx = line.end.x() - line.start.x();
    dy = line.end.y() - line.start.y();

    len = std::abs(dy) > std::abs(dx) ? std::abs(dy) : std::abs(dx);

    dx /= len;
    dy /= len;

    x = line.start.x();
    y = line.start.y();

    for (int i = 0; i < len; i++)
    {
        painter.drawPoint(std::round(x), std::round(y));
        x += dx;
        y += dy;
    }
}