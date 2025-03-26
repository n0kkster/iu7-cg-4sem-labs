#include "canonical.h"
#include <math.h>

#include <QPainter>

void drawCicrleCanonical(QPainter &painter, const circle_t &circle, bool measure_mode)
{
    int x, y;
    std::array<QPoint, NUM_TYPES> curr_points;

    double xc = circle.center.x(), yc = circle.center.y(), r = circle.radius;

    QPoint center = { lrintf64(xc), lrintf64(yc) };

    for (x = xc; x <= lrintf64(xc + r / sqrt(2)); ++x)
    {
        y = lrintf64(yc + sqrt(r * r - (x - xc) * (x - xc)));

        curr_points[CURRENT] = {x, y};
        curr_points[M_X] = mirrorPointByX(curr_points[CURRENT], center.x());
        curr_points[M_Y] = mirrorPointByY(curr_points[CURRENT], center.y());
        curr_points[M_XY] = mirrorPointByXY(curr_points[CURRENT], center);

        curr_points[M_OCTANT] = mirrorPointByOctant({x, y}, center);
        curr_points[M_X_OCTANT] = mirrorPointByX(curr_points[M_OCTANT], center.x());
        curr_points[M_Y_OCTANT] = mirrorPointByY(curr_points[M_OCTANT], center.y());
        curr_points[M_XY_OCTANT] = mirrorPointByXY(curr_points[M_OCTANT], center);

        if (!measure_mode)
            for (size_t i = 0; i < curr_points.size(); ++i)
                painter.drawPoint(curr_points[i]);
    }
}

void drawEllipseCanonical(QPainter &painter, const ellipse_t &ellipse, bool measure_mode)
{
    int x, y;
    std::array<QPoint, M_OCTANT> curr_points;

    double xc = ellipse.center.x(), yc = ellipse.center.y(), rx = ellipse.rx, ry = ellipse.ry;

    double rx_sq = pow(rx, 2), ry_sq = pow(ry, 2);

    QPoint center = { lrintf64(xc), lrintf64(yc) };

    for (x = xc; x <= xc + rx; ++x)
    {
        y = lrintf64(yc + sqrt(rx_sq * ry_sq - pow((x - xc), 2) * ry_sq) / rx);

        curr_points[CURRENT] = {x, y};
        curr_points[M_X] = mirrorPointByX(curr_points[CURRENT], lrintf64(xc));
        curr_points[M_Y] = mirrorPointByY(curr_points[CURRENT], lrintf64(yc));
        curr_points[M_XY] = mirrorPointByXY(curr_points[CURRENT], center);

        if (!measure_mode)
            for (size_t i = 0; i < curr_points.size(); i++)
                painter.drawPoint(curr_points[i]);
    }

    for (y = yc; y <= yc + ry; ++y)
    {
        x = lrintf64(xc + sqrt(rx_sq * ry_sq - pow((y - yc), 2) * rx_sq) / ry);

        curr_points[CURRENT] = {x, y};
        curr_points[M_X] = mirrorPointByX(curr_points[CURRENT], lrintf64(xc));
        curr_points[M_Y] = mirrorPointByY(curr_points[CURRENT], lrintf64(yc));
        curr_points[M_XY] = mirrorPointByXY(curr_points[CURRENT], center);

        if (!measure_mode)
            for (size_t i = 0; i < curr_points.size(); ++i)
                painter.drawPoint(curr_points[i]);
    }
}
