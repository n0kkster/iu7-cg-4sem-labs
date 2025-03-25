#include "parametric.h"
#include "math.h"

#include <QPainter>

void drawCicrleParametric(QPainter &painter, const circle_t &circle, bool measure_mode)
{
    int x, y;
    std::array<QPoint, NUM_TYPES> curr_points;

    double xc = circle.center.x(), yc = circle.center.y(), r = circle.radius;
    double step = 1 / r;

    QPoint center = { lrintf64(xc), lrintf64(yc) };

    for (double t = 0; t < M_PI / 4; t += step)
    {
        x = lrintf64(xc + r * cos(t));
        y = lrintf64(yc + r * sin(t));

        curr_points[CURRENT] = {x, y};
        curr_points[M_X] = mirrorPointByX(curr_points[CURRENT], lrintf64(xc));
        curr_points[M_Y] = mirrorPointByY(curr_points[CURRENT], lrintf64(yc));
        curr_points[M_XY] = mirrorPointByXY(curr_points[CURRENT], center);

        curr_points[M_OCTANT] = mirrorPointByOctant({x, y}, center);
        curr_points[M_X_OCTANT] = mirrorPointByX(curr_points[M_OCTANT], lrintf64(xc));
        curr_points[M_Y_OCTANT] = mirrorPointByY(curr_points[M_OCTANT], lrintf64(yc));
        curr_points[M_XY_OCTANT] = mirrorPointByXY(curr_points[M_OCTANT], center);

        if (!measure_mode)
            for (size_t i = 0; i < curr_points.size(); i++)
                painter.drawPoint(curr_points[i]);
    }
}

void drawEllipseParametric(QPainter &painter, const ellipse_t &ellipse, bool measure_mode)
{
    int x, y;
    std::array<QPoint, M_OCTANT> curr_points;

    double xc = ellipse.center.x(), yc = ellipse.center.y(), rx = ellipse.rx, ry = ellipse.ry;
    double step = std::min(1 / rx, 1 / ry);

    QPoint center = { lrintf64(xc), lrintf64(yc) };

    for (double t = 0; t < M_PI / 2; t += step)
    {
        x = lrintf64(xc + rx * cos(t));
        y = lrintf64(yc + ry * sin(t));

        curr_points[CURRENT] = {x, y};
        curr_points[M_X] = mirrorPointByX(curr_points[CURRENT], lrintf64(xc));
        curr_points[M_Y] = mirrorPointByY(curr_points[CURRENT], lrintf64(yc));
        curr_points[M_XY] = mirrorPointByXY(curr_points[CURRENT], center);

        if (!measure_mode)
            for (size_t i = 0; i < curr_points.size(); i++)
                painter.drawPoint(curr_points[i]);
    }
}
