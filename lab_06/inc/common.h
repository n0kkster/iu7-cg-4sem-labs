#ifndef COMMON_H
#define COMMON_H

#include <QVector>

#include <QPointF>

struct QPainter;

typedef struct
{
    int x, y;
} point_t;

typedef struct
{
    point_t start, end;
} edge_t;

typedef struct
{
    QVector<point_t> vertices;
    QVector<edge_t> edges;
    bool need_fill;
    bool outlined;
} shape_t;

typedef enum
{
    CURRENT, 
    M_X,
    M_Y,
    M_XY,
    M_OCTANT,
    M_X_OCTANT,
    M_Y_OCTANT,
    M_XY_OCTANT,
    NUM_TYPES
} point_type_e;

typedef struct
{
    QPointF center;
    double radius;
} circle_t;

typedef struct
{
    QPointF center;
    double rx, ry;
} ellipse_t;

QPoint mirrorPointByX(const QPoint &point, int x);
QPoint mirrorPointByY(const QPoint &point, int y);
QPoint mirrorPointByXY(const QPoint &point, const QPoint &center);
QPoint mirrorPointByOctant(const QPoint &point, const QPoint &center);

#endif /* COMMON_H */
