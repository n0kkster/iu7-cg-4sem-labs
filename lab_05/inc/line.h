#ifndef LINE_H
#define LINE_H

#include "plane.h"

struct QPainter;

int drawLine(QPainter &painter, const line_t &line, bool measure_mode=false);

#endif /* LINE_H */
