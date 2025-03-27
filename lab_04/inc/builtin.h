#ifndef BUILTIN_H
#define BUILTIN_H

#include "common.h"

void drawCircleBuiltin(QPainter &painter, const circle_t &circle, bool measure_mode = false);
void drawEllipseBuiltin(QPainter &painter, const ellipse_t &ellipse, bool measure_mode = false);

#endif /* BUILTIN_H */
