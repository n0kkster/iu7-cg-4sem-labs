#include "fill.h"

#include "common.h"

#include <QImage>
#include <QStack>

#ifndef BY_LINES 

static bool check_pixel(const point_t &pixel, const QImage &buffer, const QColor &border_color,
                        const QColor &fill_color)
{
    if (!(pixel.x >= 0 && pixel.x < buffer.width() && pixel.y >= 0 && pixel.y < buffer.height()))
        return false;

    QColor curr_color = buffer.pixelColor(pixel.x, pixel.y);

    return curr_color != fill_color && curr_color != border_color;
}
#endif

int fill(QPainter &painter, const QImage &buffer, const point_t seed, const QColor &border_color,
         const QColor &fill_color, int iter_stop)
{
    QStack<point_t> points;
    point_t curr;
    int iter = 0;

    points.push(seed);
    
#ifdef BY_LINES
    int x, y, tx, ty, xl, xr, x_in;
    bool flag;

    while (!points.empty())
    {
        curr = points.pop();

        x = curr.x;
        y = curr.y;
        tx = x;
        ty = y;

        painter.drawPoint(x, y);

        ++x;

        while (buffer.pixelColor(x, y) != fill_color && buffer.pixelColor(x, y) != border_color
               && x < buffer.width())
        {
            painter.drawPoint(x, y);
            ++x;
        }

        xr = x - 1;
        x = tx - 1;

        while (buffer.pixelColor(x, y) != fill_color && buffer.pixelColor(x, y) != border_color && x > 0)
        {
            painter.drawPoint(x, y);
            --x;
        }

        if (iter > iter_stop)
            return iter_stop;

        iter++;

        xl = x + 1;

        x = xl;
        if (ty > 0)
        {
            y = ty - 1;
            while (x <= xr)
            {
                flag = false;
                while (buffer.pixelColor(x, y) != fill_color && buffer.pixelColor(x, y) != border_color
                       && x <= xr)
                {
                    flag = true;
                    ++x;
                }

                if (flag)
                {
                    if (x == xr && buffer.pixelColor(x, y) != fill_color
                        && buffer.pixelColor(x, y) != border_color)
                    {
                        if (y > 0)
                            points.push({ x, y });
                    }
                    else
                    {
                        if (y > 0)
                            points.push({ x - 1, y });
                    }
                    flag = false;
                }

                x_in = x;
                while ((buffer.pixelColor(x, y) == fill_color || buffer.pixelColor(x, y) == border_color)
                       && x < xr)
                {
                    ++x;
                }

                if (x == x_in)
                    ++x;
            }
        }

        if (iter > iter_stop)
            return iter_stop;

        iter++;

        x = xl;
        if (ty < buffer.height())
        {
            y = ty + 1;
            while (x <= xr)
            {
                flag = false;
                while (buffer.pixelColor(x, y) != fill_color && buffer.pixelColor(x, y) != border_color
                       && x <= xr)
                {
                    flag = true;
                    ++x;
                }

                if (flag)
                {
                    if (x == xr && buffer.pixelColor(x, y) != fill_color
                        && buffer.pixelColor(x, y) != border_color)
                    {
                        if (y < buffer.height())
                            points.push({ x, y });
                    }
                    else
                    {
                        if (y < buffer.height())
                            points.push({ x - 1, y });
                    }
                    flag = false;
                }

                x_in = x;
                while ((buffer.pixelColor(x, y) == fill_color || buffer.pixelColor(x, y) == border_color)
                       && x < xr)
                {
                    ++x;
                }

                if (x == x_in)
                    ++x;
            }
        }

        if (iter > iter_stop)
            return iter_stop;

        iter++;
    }
#else
    bool need_inc;

    while (!points.empty())
    {
        need_inc = false;

        curr = points.pop();

        if (iter > iter_stop)
            return iter;

        painter.drawPoint(curr.x, curr.y);

        if (check_pixel({ curr.x + 1, curr.y }, buffer, border_color, fill_color))
        {
            points.push({ curr.x + 1, curr.y });
            need_inc = true;
        }

        if (check_pixel({ curr.x, curr.y + 1 }, buffer, border_color, fill_color))
        {
            points.push({ curr.x, curr.y + 1 });
            need_inc = true;
        }

        if (check_pixel({ curr.x - 1, curr.y }, buffer, border_color, fill_color))
        {
            points.push({ curr.x - 1, curr.y });
            need_inc = true;
        }

        if (check_pixel({ curr.x, curr.y - 1 }, buffer, border_color, fill_color))
        {
            points.push({ curr.x, curr.y - 1 });
            need_inc = true;
        }

        if (need_inc)
            iter++;
    }
#endif
    return iter;
}
