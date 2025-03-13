#ifndef BRESENHAM_H
#define BRESENHAM_H

#include <QPainter>
#include "plane.h"

void bres_real(QPainter &painter, const line_t &line, bool measure_mode=false);
void bres_int(QPainter &painter, const line_t &line, bool measure_mode=false);
void bres_smooth(QPainter &painter, const line_t &line, bool measure_mode=false);

#endif /* BRESENHAM_H */
