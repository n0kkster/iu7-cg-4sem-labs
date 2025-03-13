#ifndef DDA_H
#define DDA_H

#include <QPainter>
#include "plane.h"

void dda(QPainter &painter, const line_t &line, bool measure_mode=false);

#endif /* DDA_H */
