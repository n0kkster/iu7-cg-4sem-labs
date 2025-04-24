#include "line.h"

#include <QPainter>

void drawLine(QPainter &painter, const line_t &line)
{
    double dx, dy, len;
    double x, y;

    painter.setPen({line.color, 1});

    if (line.start.x == line.end.x && line.start.y == line.end.y)
    {
        painter.drawPoint(line.start.x, line.start.y);
        return;
    }

    dx = line.end.x - line.start.x;
    dy = line.end.y - line.start.y;

    len = std::abs(dy) > std::abs(dx) ? std::abs(dy) : std::abs(dx);

    dx /= len;
    dy /= len;

    x = line.start.x;
    y = line.start.y;

    for (int i = 0; i <= len; i++)
    {
        painter.drawPoint(lrintf64(x), lrintf64(y));

        x += dx;
        y += dy;
    }
}