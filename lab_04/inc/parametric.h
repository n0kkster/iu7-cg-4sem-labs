#ifndef PARAMETRIC_H
#define PARAMETRIC_H

#include "common.h"

struct QPainter;

void drawCicrleParametric(QPainter &painter, const circle_t &circle, bool measure_mode = false);
void drawEllipseParametric(QPainter &painter, const ellipse_t &ellipse, bool measure_mode = false);

#endif /* PARAMETRIC_H */
