#include "circle.h"

#include <QPainter>

void drawCicrle(QPainter &painter, const circle_t &circle, bool measure_mode)
{
    int x, y;
    std::array<QPoint, NUM_TYPES> curr_points;

    double xc = circle.center.x(), yc = circle.center.y(), r = circle.radius;

    QPoint center = { (int)lrintf64(xc), (int)lrintf64(yc) };

    for (x = xc; x <= lrintf64(xc + r / sqrt(2)); ++x)
    {
        y = lrintf64(yc + sqrt(r * r - (x - xc) * (x - xc)));

        if (!measure_mode)
        {
            curr_points[CURRENT] = { x, y };
            curr_points[M_X] = mirrorPointByX(curr_points[CURRENT], center.x());
            curr_points[M_Y] = mirrorPointByY(curr_points[CURRENT], center.y());
            curr_points[M_XY] = mirrorPointByXY(curr_points[CURRENT], center);

            curr_points[M_OCTANT] = mirrorPointByOctant({ x, y }, center);
            curr_points[M_X_OCTANT] = mirrorPointByX(curr_points[M_OCTANT], center.x());
            curr_points[M_Y_OCTANT] = mirrorPointByY(curr_points[M_OCTANT], center.y());
            curr_points[M_XY_OCTANT] = mirrorPointByXY(curr_points[M_OCTANT], center);
        }

        if (!measure_mode)
            for (size_t i = 0; i < curr_points.size(); ++i)
                painter.drawPoint(curr_points[i]);
    }
}
