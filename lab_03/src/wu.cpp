#include "wu.h"

void wu(QPainter &painter, const line_t &line, bool measure_mode=false)
{
    int dx, dy;
    int start, end;
    double m, ideal, frac;
    int x, y, step, steps, xi, yi;

    int xs, ys, xe, ye;

    xs = (int)round(line.start.x());
    ys = (int)round(line.start.y());

    xe = (int)round(line.end.x());
    ye = (int)round(line.end.y());

    if (xs == xe && ys == ye)
    {
        if (!measure_mode)
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
            if (!measure_mode)
                painter.drawPoint(xs, y);
    }
    else if (dy == 0)
    {
        start = std::min(line.start.x(), line.end.x());
        end = std::max(line.start.x(), line.end.x());
        for (int x = start; x <= end; x++)
            if (!measure_mode)
                painter.drawPoint(x, ys);
    }
    else
    {
        if (std::abs(dx) >= std::abs(dy))
        {
            m = (double)dy / dx;
            ideal = line.start.y();
            x = line.start.x();
            steps = std::abs(dx);
            step = (dx > 0) ? 1 : -1;

            for (int i = 0; i <= steps; i++)
            {
                yi = (int)floor(ideal);
                frac = ideal - yi;
                
                if (!measure_mode)
                {
                    painter.setOpacity(1 - frac);
                    painter.drawPoint(x, yi);

                    painter.setOpacity(frac);
                    painter.drawPoint(x, yi + 1);
                }

                x += step;
                ideal += m * step;
            }
        }
        else
        {
            m = (double)dx / dy;
            ideal = line.start.x();
            y = line.start.y();
            steps = std::abs(dy);
            step = (dy > 0) ? 1 : -1;

            for (int i = 0; i <= steps; i++)
            {
                xi = (int)floor(ideal);
                frac = ideal - xi;

                if (!measure_mode)
                {
                    painter.setOpacity(1 - frac);
                    painter.drawPoint(xi, y);

                    painter.setOpacity(frac);
                    painter.drawPoint(xi + 1, y);
                }

                y += step;
                ideal += m * step;
            }
        }
    }
    painter.setOpacity(1);
}