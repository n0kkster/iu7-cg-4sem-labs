#ifndef BRESENHAM_H
#define BRESENHAM_H

#include <QPainter>
#include "plane.h"

void drawBresCircle(QPainter &painter, const circle_t &circle, bool measure_mode=false);
void drawBresEllipse(QPainter &painter, const ellipse_t &ellipse, bool measure_mode=false);

#endif /* BRESENHAM_H */
