#ifndef MIDPOINT_H
#define MIDPOINT_H

#include "common.h"

void drawCicrleMidpoint(QPainter &painter, const circle_t &circle, bool measure_mode = false);
void drawEllipseMidpoint(QPainter &painter, const ellipse_t &ellipse, bool measure_mode = false);

#endif /* MIDPOINT_H */
