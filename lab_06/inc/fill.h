#ifndef FILL_H
#define FILL_H

#include "common.h"

#include <QColor>
#include <QPainter>
#include <QVector>

typedef struct
{
    int xmin, xmax;
    int ymin, ymax;
} dimensions_t;

void outline(QPainter &painter, std::map<int, std::vector<point_t>> &outline_points, const shape_t &shape,
             const dimensions_t &dim);
void fill(QPainter &painter, const std::map<int, std::vector<point_t>> &outline_points, const QColor &color,
          const dimensions_t &dim, bool delayEnabled = false, int stop_line = 0);
void updateDimensions(dimensions_t &dim, const shape_t &shape);

#endif /* FILL_H */
