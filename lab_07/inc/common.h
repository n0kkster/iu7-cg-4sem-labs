#ifndef COMMON_H
#define COMMON_H

#include <QColor>

typedef struct
{
    int x, y;
} point_t;

typedef struct
{
    point_t start, end;
    QColor color;
} line_t;

#endif /* COMMON_H */
