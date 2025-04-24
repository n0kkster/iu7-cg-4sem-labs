#ifndef CUT_H
#define CUT_H

#include "common.h"

struct QPainter;
struct QRect;

void cut(QPainter &painter, const QRect &rect, line_t line, QColor resColor);

#endif /* CUT_H */
