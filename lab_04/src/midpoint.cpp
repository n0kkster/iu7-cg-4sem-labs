#include "midpoint.h"
#include <math.h>

#include <QPainter>

void drawCicrleMidpoint(QPainter &painter, const circle_t &circle, bool measure_mode)
{
    int x, y;
    std::array<QPoint, NUM_TYPES> curr_points;

    double xc = circle.center.x(), yc = circle.center.y(), r = circle.radius;
    int p = 1 - r;

    x = r;
    y = 0;

    QPoint center = { lrintf64(xc), lrintf64(yc) };

    while (x >= y)
    {
        curr_points[CURRENT] = {x + center.x(), y + center.y()};
        curr_points[M_X] = mirrorPointByX(curr_points[CURRENT], lrintf64(xc));
        curr_points[M_Y] = mirrorPointByY(curr_points[CURRENT], lrintf64(yc));
        curr_points[M_XY] = mirrorPointByXY(curr_points[CURRENT], center);

        curr_points[M_OCTANT] = mirrorPointByOctant(curr_points[CURRENT], center);
        curr_points[M_X_OCTANT] = mirrorPointByX(curr_points[M_OCTANT], lrintf64(xc));
        curr_points[M_Y_OCTANT] = mirrorPointByY(curr_points[M_OCTANT], lrintf64(yc));
        curr_points[M_XY_OCTANT] = mirrorPointByXY(curr_points[M_OCTANT], center);

        if (p < 0)
        {
            ++y;
            p += 2 * y + 1;
        }
        else
        {
            ++y;
            --x;
            p += 2 * y + 1 - 2 * x;
        }

        if (!measure_mode)
            for (size_t i = 0; i < curr_points.size(); i++)
                painter.drawPoint(curr_points[i]);
    }
}

void drawEllipseMidpoint(QPainter &painter, const ellipse_t &ellipse, bool measure_mode)
{
    int x, y;
    std::array<QPoint, M_OCTANT> curr_points;

    double xc = ellipse.center.x(), yc = ellipse.center.y(), rx = ellipse.rx, ry = ellipse.ry;
    double p;

    double rx_sq = pow(rx, 2), ry_sq = pow(ry, 2);

    QPoint center = { lrintf64(xc), lrintf64(yc) };

    x = 0;
    y = ry;

    p = ry_sq - lrintf64(rx_sq * (ry - 1 / 4));

    while (x <= lrintf64(rx / sqrt(1 + ry_sq / rx_sq)))
    {
        curr_points[CURRENT] = { x + center.x(), y + center.y() };
        curr_points[M_X] = mirrorPointByX(curr_points[CURRENT], lrintf64(xc));
        curr_points[M_Y] = mirrorPointByY(curr_points[CURRENT], lrintf64(yc));
        curr_points[M_XY] = mirrorPointByXY(curr_points[CURRENT], center);

        if (p < 0)
        {
            ++x;
            p += ry_sq * 2 * x + ry_sq;
        }
        else
        {
            ++x;
            --y;
            p += ry_sq * 2 * x - rx_sq * 2 * y + ry_sq;
        }

        if (!measure_mode)
            for (size_t i = 0; i < curr_points.size(); i++)
                painter.drawPoint(curr_points[i]);
    }

    x = rx;
    y = 0;
    
    p = rx_sq - lrintf64(ry_sq * (rx - 1 / 4));

    while (y <= lrintf64(ry / sqrt(1 + rx_sq / ry_sq)))
    {
        curr_points[CURRENT] = { x + center.x(), y + center.y() };
        curr_points[M_X] = mirrorPointByX(curr_points[CURRENT], lrintf64(xc));
        curr_points[M_Y] = mirrorPointByY(curr_points[CURRENT], lrintf64(yc));
        curr_points[M_XY] = mirrorPointByXY(curr_points[CURRENT], center);

        if (p < 0)
        {
            ++y;
            p += rx_sq * 2 * y + rx_sq;
        }
        else
        {
            --x;
            ++y;
            p += rx_sq * 2 * y - ry_sq * 2 * x + rx_sq;
        }

        if (!measure_mode)
            for (size_t i = 0; i < curr_points.size(); i++)
                painter.drawPoint(curr_points[i]);
    }
}
