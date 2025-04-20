#include "common.h"

QPoint mirrorPointByX(const QPoint &point, int x)
{
    return { 2 * x - point.x(), point.y() };
}

QPoint mirrorPointByY(const QPoint &point, int y)
{
    return { point.x(), 2 * y - point.y() };
}

QPoint mirrorPointByXY(const QPoint &point, const QPoint &center)
{
    return mirrorPointByX(mirrorPointByY(point, center.y()), center.x());
}

QPoint mirrorPointByOctant(const QPoint &point, const QPoint &center)
{
    const int dx = center.x() - point.x();
    const int dy = center.y() - point.y();

    return { center.x() + dy, center.y() + dx };
}
