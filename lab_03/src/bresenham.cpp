#include "bresenham.h"

int sign(double x)
{
    if (x > 1e-9)
        return 1;

    if (x < -1e-9)
        return -1;

    return 0;
}

void bres_real(QPainter &painter, const line_t &line)
{
    double m, f;
    int sx, sy, dx, dy, x, y;
    bool fl = false;

    dx = line.end.x() - line.start.x();
    dy = line.end.y() - line.start.y();

    if (std::abs(dx) < 1e-9 && std::abs(dy) < 1e-9)
    {
        painter.drawPoint(line.start.x(), line.start.y());
        return;
    }

    sx = sign(dx);
    sy = sign(dy);

    dx = std::abs(dx);
    dy = std::abs(dy);

    m = dy / (double)dx;

    if (m > 1)
    {
        std::swap(dx, dy);
        m = 1 / m;
        fl = true;
    }


    f = m - 0.5;
    x = line.start.x();
    y = line.start.y();

    for (int i = 0; i <= dx; i++)
    {
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
}

void bres_int(QPainter &painter, const line_t &line)
{
    int sx, sy, dx, dy, x, y, f;
    bool fl = false;

    dx = line.end.x() - line.start.x();
    dy = line.end.y() - line.start.y();

    if (std::abs(dx) < 1e-9 && std::abs(dy) < 1e-9)
    {
        painter.drawPoint(line.start.x(), line.start.y());
        return;
    }

    sx = sign(dx);
    sy = sign(dy);

    dx = std::abs(dx);
    dy = std::abs(dy);

    if (dy > dx)
    {
        std::swap(dx, dy);
        fl = true;
    }

    f = 2 * dy - dx;
    x = line.start.x();
    y = line.start.y();

    for (int i = 0; i <= dx; i++)
    {
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
}

void bres_smooth(QPainter &painter, const line_t &line)
{
    double m, f, w;
    int sx, sy, dx, dy, x, y, I = 8;
    bool fl = false;

    dx = line.end.x() - line.start.x();
    dy = line.end.y() - line.start.y();

    if (std::abs(dx) < 1e-9 && std::abs(dy) < 1e-9)
    {
        painter.drawPoint(line.start.x(), line.start.y());
        return;
    }

    sx = sign(dx);
    sy = sign(dy);

    dx = std::abs(dx);
    dy = std::abs(dy);

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

    x = line.start.x();
    y = line.start.y();

    qDebug() << dx << dy << m << f << w << I << sx << sy;

    painter.setOpacity(f / I);
    painter.drawPoint(x, y);

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

        painter.setOpacity(f / I);
        painter.drawPoint(x, y);
    }

    painter.setOpacity(1);
}
