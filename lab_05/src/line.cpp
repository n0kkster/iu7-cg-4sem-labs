#include "line.h"

#include <QPainter>

int drawLine(QPainter &painter, const line_t &line, bool measure_mode)
{
    double dx, dy, len;
    double x, y;
    int steps;

    if (line.start == line.end)
    {
        if (!measure_mode)
            painter.drawPoint(line.start.x(), line.start.y());
        return 0;
    }

    dx = line.end.x() - line.start.x();
    dy = line.end.y() - line.start.y();

    steps = (int)std::min(std::abs(dx), std::abs(dy));

    len = std::abs(dy) > std::abs(dx) ? std::abs(dy) : std::abs(dx);

    dx /= len;
    dy /= len;

    x = line.start.x();
    y = line.start.y();

    for (int i = 0; i <= len; i++)
    {
        if (!measure_mode)
            painter.drawPoint(lrintf64(x), lrintf64(y));

        x += dx;
        y += dy;
    }

    return steps;
}