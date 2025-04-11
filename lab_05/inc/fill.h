#ifndef FILL_H
#define FILL_H

#include <QVector>
#include <QPainter>
#include <QColor>
#include "common.h"

typedef struct
{
    int xmin, xmax;
    int ymin, ymax;
} dimensions_t;

void outline(QPainter &painter, QVector<point_t> &outline_points, const shape_t &shape, const dimensions_t &dim);
void fill(QPainter &painter, const QVector<point_t> &outline_points, const QColor &color, const dimensions_t &dim);
void updateDimensions(dimensions_t &dim, const shape_t &shape);

#endif /* FILL_H */
