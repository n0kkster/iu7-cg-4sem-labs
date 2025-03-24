#ifndef BRESENHAM_H
#define BRESENHAM_H

#include <QPainter>
#include "plane.h"

int bres_circle(QPainter &painter, const circle_t &circle, bool measure_mode=false);
int bres_ellipse(QPainter &painter, const ellipse_t &ellipse, bool measure_mode=false);

#endif /* BRESENHAM_H */
