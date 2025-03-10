#include <QPaintEvent>
#include <QDebug>
#include <QPen>
#include <QPalette>

#include "plane.h"

// Конструктор
// ==================================================
Plane::Plane(QWidget *parent) : QGraphicsView(parent) 
{
    color.setRgb(39, 40, 41);
}
// ==================================================

void Plane::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(viewport());

    for (const auto& line : lines)
        myDrawLine(painter, line);
}

void Plane::myDrawLine(QPainter &painter, const line_t &line)
{
    painter.setPen(line.color);
    switch (line.method)
    {
        case BUILTIN:
            painter.drawLine(line.start, line.end);
            break;

        case DDA:
            dda(painter, line);
            break;
        
        case BRES_REAL:
            break;
        
        case BRES_INT:
            break;
        
        case BRES:
            break;

        case WU:
            break;
        
        default:
            break;
    }
}

void Plane::addLine(line_t line)
{
    lines.append(line);
}

void Plane::setBGColor(const QColor &color)
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Base, color);
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    this->viewport()->update();
}
