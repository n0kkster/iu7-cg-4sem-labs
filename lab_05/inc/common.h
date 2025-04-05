#ifndef COMMON_H
#define COMMON_H

#include <QVector>

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
} shape_t;


#endif /* COMMON_H */
