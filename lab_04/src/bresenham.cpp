#include "bresenham.h"

void drawCircleBres(QPainter &painter, const circle_t &circle, bool measure_mode)
{
    int x, y;
    std::array<QPoint, NUM_TYPES> curr_points;

    double xc = circle.center.x(), yc = circle.center.y(), r = circle.radius;
    int delta = 2 * (1 - r), d;

    x = 0;
    y = r;

    QPoint center = { lrintf64(xc), lrintf64(yc) };

    while (x <= y)
    {
        curr_points[CURRENT] = {x + center.x(), y + center.y()};
        curr_points[M_X] = mirrorPointByX(curr_points[CURRENT], center.x());
        curr_points[M_Y] = mirrorPointByY(curr_points[CURRENT], center.y());
        curr_points[M_XY] = mirrorPointByXY(curr_points[CURRENT], center);

        curr_points[M_OCTANT] = mirrorPointByOctant(curr_points[CURRENT], center);
        curr_points[M_X_OCTANT] = mirrorPointByX(curr_points[M_OCTANT], center.x());
        curr_points[M_Y_OCTANT] = mirrorPointByY(curr_points[M_OCTANT], center.y());
        curr_points[M_XY_OCTANT] = mirrorPointByXY(curr_points[M_OCTANT], center);

        d = 2 * (delta + y) - 1;
        ++x;

        if (d < 0)
            delta += 2 * x + 1;
        else
        {
            --y;
            delta += 2 * (x - y + 1);
        }

        if (!measure_mode)
            for (size_t i = 0; i < curr_points.size(); i++)
                painter.drawPoint(curr_points[i]);
    }
}

void drawEllipseBres(QPainter &painter, const ellipse_t &ellipse, bool measure_mode)
{
    int x, y;
    std::array<QPoint, M_OCTANT> curr_points;

    double xc = ellipse.center.x(), yc = ellipse.center.y(), rx = ellipse.rx, ry = ellipse.ry;
    double rx_sq = pow(rx, 2), ry_sq = pow(ry, 2);
    int delta = ry_sq - rx_sq * (2 * ry - 1), d1, d2;

    QPoint center = { lrintf64(xc), lrintf64(yc) };

    x = 0;
    y = ry;

    while (y >= 0)
    {
        curr_points[CURRENT] = { x + center.x(), y + center.y() };
        curr_points[M_X] = mirrorPointByX(curr_points[CURRENT], center.x());
        curr_points[M_Y] = mirrorPointByY(curr_points[CURRENT], center.y());
        curr_points[M_XY] = mirrorPointByXY(curr_points[CURRENT], center);

        if (delta < 0)
        {
            d1 = 2 * delta + rx_sq * (2 * y + 1);
            ++x;

            if (d1 < 0)
                delta += ry_sq * (2 * x + 1);
            else
            {
                --y;
                delta += ry_sq * (2 * x + 1) + rx_sq * (1 - 2 * y);
            }
        }
        else if (delta > 0)
        {
            d2 = 2 * delta + ry_sq * (2 - 2 * x);
            --y;

            if (d2 > 0)
                delta += rx_sq * (1 - 2 * y);
            else
            {
                ++x;
                delta += ry_sq * (2 * x + 1) + rx_sq * (1 - 2 * y);
            }
        }
        else
        {
            --y;
            ++x;
            delta += ry_sq * (2 * x + 1) + rx_sq * (1 - 2 * y);
        }

        if (!measure_mode)
            for (size_t i = 0; i < curr_points.size(); i++)
                painter.drawPoint(curr_points[i]);
    }
}
