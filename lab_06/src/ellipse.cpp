#include "ellipse.h"

#include <QPainter>

void drawEllipse(QPainter &painter, const ellipse_t &ellipse)
{
    int x, y;
    std::array<QPoint, M_OCTANT> curr_points;

    double xc = ellipse.center.x(), yc = ellipse.center.y(), rx = ellipse.rx, ry = ellipse.ry;

    double rx_sq = pow(rx, 2), ry_sq = pow(ry, 2);

    QPoint center = { (int)lrintf64(xc), (int)lrintf64(yc) };

    for (x = xc; x <= xc + rx; ++x)
    {
        y = lrintf64(yc + sqrt(rx_sq * ry_sq - pow((x - xc), 2) * ry_sq) / rx);

        curr_points[CURRENT] = { x, y };
        curr_points[M_X] = mirrorPointByX(curr_points[CURRENT], lrintf64(xc));
        curr_points[M_Y] = mirrorPointByY(curr_points[CURRENT], lrintf64(yc));
        curr_points[M_XY] = mirrorPointByXY(curr_points[CURRENT], center);

        for (size_t i = 0; i < curr_points.size(); i++)
            painter.drawPoint(curr_points[i]);
    }

    for (y = yc; y <= yc + ry; ++y)
    {
        x = lrintf64(xc + sqrt(rx_sq * ry_sq - pow((y - yc), 2) * rx_sq) / ry);

        curr_points[CURRENT] = { x, y };
        curr_points[M_X] = mirrorPointByX(curr_points[CURRENT], lrintf64(xc));
        curr_points[M_Y] = mirrorPointByY(curr_points[CURRENT], lrintf64(yc));
        curr_points[M_XY] = mirrorPointByXY(curr_points[CURRENT], center);

        for (size_t i = 0; i < curr_points.size(); ++i)
            painter.drawPoint(curr_points[i]);
    }
}
