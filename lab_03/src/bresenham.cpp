#include "bresenham.h"

int sign(double x)
{
    if (x > 1e-9)
        return 1;

    if (x < -1e-9)
        return -1;

    return 0;
}

int bres_real(QPainter &painter, const line_t &line, bool measure_mode)
{
    double m, f;
    int sx, sy, dx, dy, x, y;
    bool fl = false;

    int xs, ys, xe, ye;

    int steps;

    xs = (int)round(line.start.x());
    ys = (int)round(line.start.y());

    xe = (int)round(line.end.x());
    ye = (int)round(line.end.y());

    if (xs == xe && ys == ye)
    {
        if (!measure_mode)
            painter.drawPoint(xs, ys);

        return 0;
    }

    dx = xe - xs;
    dy = ye - ys;

    sx = sign(dx);
    sy = sign(dy);

    dx = std::abs(dx);
    dy = std::abs(dy);

    steps = (int)std::min(dx, dy);

    m = dy / (double)dx;

    if (m > 1)
    {
        std::swap(dx, dy);
        m = 1 / m;
        fl = true;
    }


    f = m - 0.5;
    x = xs;
    y = ys;

    for (int i = 0; i <= dx; i++)
    {
        if (!measure_mode)
            painter.drawPoint(x, y);

        if (f >= 0)
        {
            if (fl)
                x += sx;
            else
                y += sy;
            f -= 1; 
        }

        if (f < 0)
        {
            if (fl)
                y += sy;
            else
                x += sx;
            f += m;
        }
    }

    return steps;
}

int bres_int(QPainter &painter, const line_t &line, bool measure_mode)
{
    int sx, sy, dx, dy, x, y, f;
    bool fl = false;

    int xs, ys, xe, ye;

    int steps;

    xs = (int)round(line.start.x());
    ys = (int)round(line.start.y());

    xe = (int)round(line.end.x());
    ye = (int)round(line.end.y());

    if (xs == xe && ys == ye)
    {
        if (!measure_mode)
            painter.drawPoint(xs, ys);

        return 0;
    }

    dx = xe - xs;
    dy = ye - ys;

    sx = sign(dx);
    sy = sign(dy);

    dx = std::abs(dx);
    dy = std::abs(dy);

    steps = (int)std::min(dx, dy);

    if (dy > dx)
    {
        std::swap(dx, dy);
        fl = true;
    }

    f = 2 * dy - dx;
    x = xs;
    y = ys;

    for (int i = 0; i <= dx; i++)
    {
        if (!measure_mode)
            painter.drawPoint(x, y);

        if (f >= 0)
        {
            if (fl)
                x += sx;
            else
                y += sy;
            f -= 2 * dx; 
        }
        
        if (f < 0)
        {
            if (fl)
                y += sy;
            else
                x += sx;
            f += 2 * dy;
        }
    }

    return steps;
}

int bres_smooth(QPainter &painter, const line_t &line, bool measure_mode)
{
    double m, f, w;
    int sx, sy, dx, dy, x, y, I = 100;
    bool fl = false;

    int xs, ys, xe, ye;

    int steps;

    xs = (int)round(line.start.x());
    ys = (int)round(line.start.y());

    xe = (int)round(line.end.x());
    ye = (int)round(line.end.y());

    if (xs == xe && ys == ye)
    {
        if (!measure_mode)
            painter.drawPoint(xs, ys);

        return 0;
    }

    dx = xe - xs;
    dy = ye - ys;

    sx = sign(dx);
    sy = sign(dy);

    dx = std::abs(dx);
    dy = std::abs(dy);

    steps = (int)std::min(dx, dy);

    m = dy / (double)dx;

    if (m > 1)
    {
        std::swap(dx, dy);
        m = 1 / m;
        fl = true;
    }

    m *= (double)I;
    f = I / 2.0;
    w = I - m;

    x = xs;
    y = ys;

    if (!measure_mode)
    {
        painter.setOpacity(f / I);
        painter.drawPoint(x, y);
    }

    for (int i = 0; i <= dx; i++)
    {
        if (f < w)
        {
            if (fl)
                y += sy;
            else
                x += sx;
            f += m;
        }

        else if (f >= w)
        {
            x += sx;
            y += sy;
            f -= w; 
        }

        if (!measure_mode)
        {
            painter.setOpacity(f / I);
            painter.drawPoint(x, y);
        }
    }

    if (!measure_mode)
        painter.setOpacity(1);

    return steps;
}
