#include "dda.h"

void dda(QPainter &painter, const line_t &line)
{
    double dx, dy, len;
    double x, y;

    dx = line.end.x() - line.start.x();
    dy = line.end.y() - line.start.y();

    len = std::abs(dy) > std::abs(dx) ? std::abs(dy) : std::abs(dx);

    if (std::abs(dx) < 1e-9 && std::abs(dy) < 1e-9)
    {
        painter.drawPoint(line.start.x(), line.start.y());
        return;
    }

    dx /= len;
    dy /= len;

    x = line.start.x();
    y = line.start.y();

    for (int i = 0; i <= len; i++)
    {
        painter.drawPoint(lrintf64(x), lrintf64(y));
        x += dx;
        y += dy;
    }
}