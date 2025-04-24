#include "plane.h"

#include "cut.h"
#include "line.h"

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

    painter.setPen({ lineColor, 1 });

    if (lineEnterStarted)
        painter.drawPoint(line_start);

    for (const auto &line : lines)
    {
        painter.setPen({ line.color, 1 });
        drawLine(painter, line);
    }

    painter.setPen({ rectColor, 1 });
    painter.drawRect(rect);

    if (needCut)
    {
        if (!rect.isEmpty())
        {
            qDebug() << "rect is not empty, cutting..";
            for (const line_t &line : lines)
                cut(painter, rect, line, resColor);
        }

        needCut = false;
    }
}

void Plane::mousePressEvent(QMouseEvent *event)
{
    Qt::KeyboardModifiers m;
    bool Z = false;
    QPoint curr = event->pos();

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
                if (Z) // TODO пофиксить, что-то не так с вычислениями
                {
                    int dx, dy;
                    int lx = line_start.x(), ly = line_start.y();

                    dx = std::abs(curr.x() - lx);
                    dy = std::abs(curr.x() - ly);

                    if (dx > dy)
                        curr.setX(lx);
                    else
                        curr.setY(ly);
                }

                addLine({ line_start.x(), line_start.y(), curr.x(), curr.y(), lineColor });
            }

            break;

        case Qt::RightButton:
            rect_start = event->pos();
            break;

        default:
            break;
    }

    viewport()->update();
}

void Plane::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::RightButton))
        return;

    addRect({ rect_start, event->pos() });

    viewport()->update();
}

// ~================ СОБЫТИЯ ================~

void Plane::addLine(const line_t &line) { lines.append(line); }

void Plane::addRect(const QRect &_rect) { rect = _rect; }

void Plane::clearPlane()
{
    lines.clear();
    res_lines.clear();

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
