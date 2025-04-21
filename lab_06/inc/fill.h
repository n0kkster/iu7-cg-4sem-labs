#ifndef FILL_H
#define FILL_H

#include "common.h"

#include <QColor>
#include <QPainter>
#include <QVector>

#define BY_LINES

int fill(QPainter &painter, const QImage &buffer, const point_t seed, const QColor &border_color,
           const QColor &fill_color, int iter_stop = -1);

#endif /* FILL_H */
