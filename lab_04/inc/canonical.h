#ifndef CANONICAL_H
#define CANONICAL_H

#include "common.h"

struct QPainter;

void drawCicrleCanonical(QPainter &painter, const circle_t &circle, bool measure_mode = false);
void drawEllipseCanonical(QPainter &painter, const ellipse_t &ellipse, bool measure_mode = false);

#endif /* CANONICAL_H */
