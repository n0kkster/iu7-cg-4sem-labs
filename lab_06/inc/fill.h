#ifndef FILL_H
#define FILL_H

#include "common.h"

#include <QColor>
#include <QPainter>
#include <QVector>


void fill2(QPainter &painter, const QImage &buffer, const point_t seed, const QColor &border_color,
           const QColor &fill_color);

#endif /* FILL_H */
