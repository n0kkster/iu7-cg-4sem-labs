#ifndef CUT_H
#define CUT_H

#include "common.h"

struct QPainter;
struct QRect;

void cut(QPainter &painter, const shape_t &clipper, const line_t &line, const QColor &resColor);

#endif /* CUT_H */
