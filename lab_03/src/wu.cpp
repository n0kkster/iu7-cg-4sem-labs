#include "wu.h"

void wu(QPainter &painter, const line_t &line)
{
    int dx, dy;
    int start, end;
    double m, ideal, frac;
    int x, y, step, steps, xi, yi;

    int xs, ys, xe, ye;

    xs = static_cast<int>(line.start.x());
    ys = static_cast<int>(line.start.y());

    xe = static_cast<int>(line.end.x());
    ye = static_cast<int>(line.end.y());

    if (xs == xe && ys == ye)
    {
        painter.drawPoint(xs, ys);
        return;
    }

    dx = xe - xs;
    dy = ye - ys;

    if (dx == 0)
    {
        start = std::min(line.start.y(), line.end.y());
        end = std::max(line.start.y(), line.end.y());
        for (int y = start; y <= end; y++)
            painter.drawPoint(xs, y);
    }
    else if (dy == 0)
    {
        start = std::min(line.start.x(), line.end.x());
        end = std::max(line.start.x(), line.end.x());
        for (int x = start; x <= end; x++)
            painter.drawPoint(x, ys);
    }
    else
    {
        if (std::abs(dx) >= std::abs(dy))
        {
            m = static_cast<double>(dy) / dx;
            ideal = static_cast<double>(line.start.y());
            x = line.start.x();
            steps = std::abs(dx);
            step = (dx > 0) ? 1 : -1;

            for (int i = 0; i <= steps; i++)
            {
                yi = static_cast<int>(std::floor(ideal));
                frac = ideal - yi;

                painter.setOpacity(1 - frac);
                painter.drawPoint(x, yi);

                painter.setOpacity(frac);
                painter.drawPoint(x, yi + 1);

                x += step;
                ideal += m * step;
            }
        }
        else
        {
            m = static_cast<double>(dx) / dy;
            ideal = static_cast<double>(line.start.x());
            y = line.start.y();
            steps = std::abs(dy);
            step = (dy > 0) ? 1 : -1;

            for (int i = 0; i <= steps; i++)
            {
                xi = static_cast<int>(std::floor(ideal));
                frac = ideal - xi;

                painter.setOpacity(1 - frac);
                painter.drawPoint(xi, y);

                painter.setOpacity(frac);
                painter.drawPoint(xi + 1, y);

                y += step;
                ideal += m * step;
            }
        }
    }
}