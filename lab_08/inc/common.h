#ifndef COMMON_H
#define COMMON_H

#include <QColor>

typedef struct
{
    double x, y;
} point_t;

typedef struct
{
    point_t start, end;
    QColor color;
} line_t;

typedef struct
{
    QVector<point_t> vertices;
    QVector<line_t> edges;
} shape_t;

#endif /* COMMON_H */
