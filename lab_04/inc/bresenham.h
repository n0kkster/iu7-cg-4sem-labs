#ifndef BRESENHAM_H
#define BRESENHAM_H

#include <QPainter>
#include "plane.h"

void drawCircleBres(QPainter &painter, const circle_t &circle, bool measure_mode=false);
void drawEllipseBres(QPainter &painter, const ellipse_t &ellipse, bool measure_mode=false);

#endif /* BRESENHAM_H */
