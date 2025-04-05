#ifndef LINE_H
#define LINE_H

#include "plane.h"
#include "common.h"

struct QPainter;

void drawLine(QPainter &painter, const edge_t &line);

#endif /* LINE_H */
