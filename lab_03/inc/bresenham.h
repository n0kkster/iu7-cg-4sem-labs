#ifndef BRESENHAM_H
#define BRESENHAM_H

#include <QPainter>
#include "plane.h"

void bres_real(QPainter &painter, const line_t &line);
void bres_int(QPainter &painter, const line_t &line);
void bres_smooth(QPainter &painter, const line_t &line);

#endif /* BRESENHAM_H */
