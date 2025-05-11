#include "plane.h"

#include "common.h"
#include "cut.h"
#include "line.h"
#include "qnamespace.h"

#include <QDebug>
#include <QGuiApplication>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPen>
#include <QTimer>

// Конструктор
// ==================================================
Plane::Plane(QWidget *parent) : QGraphicsView(parent)
{
    lineColor = QColor::fromRgb(246, 240, 240);
    rectColor = QColor::fromRgb(126, 172, 181);
    resColor = QColor::fromRgb(201, 104, 104);

    shapeConnected = false;
    lineEnterStarted = false;
    needCut = false;
}

// ==================================================

// ================= СОБЫТИЯ =================
void Plane::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(viewport());

    drawAxis(painter);

    painter.setPen(lineColor);

    if (lineEnterStarted)
        painter.drawPoint(line_start);

    for (const auto &line : lines)
    {
        painter.setPen(line.color);
        drawLine(painter, line);
    }

    painter.setPen({ rectColor, 1 });
    for (const point_t &vertex : cutter.vertices)
        painter.drawPoint(vertex.x, vertex.y);

    for (const line_t &line : cutter.edges)
        drawLine(painter, line);

    if (needCut && shapeConnected)
    {
        for (const line_t &line : lines)
            cut(painter, cutter, line, resColor);
        needCut = false;
    }
}

void Plane::mousePressEvent(QMouseEvent *event)
{
    Qt::KeyboardModifiers m;
    bool Z = false;
    QPointF curr = event->pos();

    switch (event->button())
    {
        case Qt::LeftButton:
            m = QGuiApplication::keyboardModifiers();
            if (m.testFlag(Qt::ControlModifier))
                Z = true;

            if (!lineEnterStarted)
            {
                lineEnterStarted = true;
                line_start = event->pos();
            }
            else
            {
                lineEnterStarted = false;
                if (Z)
                {
                    int dx, dy;
                    int lx = line_start.x(), ly = line_start.y();

                    dx = std::abs(curr.x() - lx);
                    dy = std::abs(curr.x() - ly);

                    if (dx < dy)
                        curr.setX(lx);
                    else
                        curr.setY(ly);
                }

                addLine({ line_start.x(), line_start.y(), curr.x(), curr.y(), lineColor });
            }

            break;

        case Qt::RightButton:
            if (!shapeConnected)
                appendToShape(event->pos(), false);
            break;

        case Qt::MiddleButton:
            if (!shapeConnected)
                connectShape();
            break;

        default:
            break;
    }

    viewport()->update();
}

// ~================ СОБЫТИЯ ================~

void Plane::addShapePoint(const point_t &point)
{
    appendToShape({ (int)point.x, (int)point.y }, false);
}

bool Plane::appendToShape(const QPoint &vertex, bool Z)
{
    point_t _vertex = { (double)vertex.x(), (double)vertex.y() };

    for (const auto &v : cutter.vertices)
        if (v.x == _vertex.x && v.y == _vertex.y)
            return false;

    if (Z && !cutter.vertices.empty())
    {
        int dx, dy;
        int lx = cutter.vertices.last().x, ly = cutter.vertices.last().y;

        dx = std::abs(_vertex.x - lx);
        dy = std::abs(_vertex.y - ly);

        if (dx < dy)
            _vertex.x = lx;
        else
            _vertex.y = ly;
    }

    if (cutter.vertices.size() > 0)
        cutter.edges.append({ cutter.vertices.last(), _vertex });
    cutter.vertices.append(_vertex);

    return true;
}

bool Plane::connectShape()
{
    if (cutter.vertices.size() <= 2)
    {
        QMessageBox::critical(this, "Ошибка",
                              "Невозможно замкнуть фигуру. Фигура должна "
                              "содержать не менее трех точек!");
        return false;
    }

    cutter.edges.append({ cutter.vertices.last(), cutter.vertices.first() });
    shapeConnected = true;
    return true;
}

void Plane::addLine(const line_t &line)
{
    lines.append(line);
}

void Plane::clearPlane()
{
    lines.clear();
    cutter.edges.clear();
    cutter.vertices.clear();

    shapeConnected = false;

    viewport()->update();
}

// ================= РИСОВАЛКИ =================
void Plane::drawAxis(QPainter &painter)
{
    const int w = viewport()->width();
    const int h = viewport()->height();

    const int span = 50;

    painter.setPen({ Qt::gray, 2 });

    painter.drawLine(0, 0, w, 0);
    painter.drawLine(0, 0, 0, h);

    for (int i = span; i <= w; i += span)
    {
        painter.drawLine(i, 0, i, 10);
        painter.drawText(i - 23, 16, QString().number(i));
    }

    for (int i = span; i <= h; i += span)
    {
        painter.drawLine(0, i, 10, i);
        painter.drawText(5, i - 10, QString().number(i));
    }
}

// ~================ РИСОВАЛКИ ================~
