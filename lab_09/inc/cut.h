#ifndef CUT_H
#define CUT_H

#include "common.h"

struct QPainter;
struct QRect;

bool cut(QPainter &painter, const shape_t &cutter, const shape_t &shape, const QColor &resColor);

#endif /* CUT_H */
