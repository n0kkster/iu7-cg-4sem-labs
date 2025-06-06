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

Plane::Plane(QWidget *parent) : QGraphicsView(parent)
{
    shapeColor = QColor::fromRgb(246, 240, 240);
    cutterColor = QColor::fromRgb(126, 172, 181);
    resColor = QColor::fromRgb(201, 104, 104);

    shapeConnected = false;
    cutterConnected = false;
    needCut = false;
}

void Plane::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(viewport());

    drawAxis(painter);

    painter.setPen(shapeColor);

    for (const line_t &line : shape.edges)
        drawLine(painter, line);

    painter.setPen(cutterColor);

    for (const line_t &line : cutter.edges)
        drawLine(painter, line);

    if (needCut)
    {
        if (shapeConnected && cutterConnected)
        {
            if (!cut(painter, cutter, shape, resColor))
                QMessageBox::critical(this, "Ошибка!", "Отсекатель не является выпуклым многоугольником!");
        }
        else if (!cutterConnected)
            QMessageBox::critical(this, "Ошибка!", "Отсекатель не добавлен или не замкнут!");
        else if (!shapeConnected)
            QMessageBox::critical(this, "Ошибка!", "Многоугольник не добавлен или не замкнут!");
        needCut = false;
    }
}

void Plane::mousePressEvent(QMouseEvent *event)
{
    Qt::KeyboardModifiers m;
    bool CTRL = false, SHIFT = false;

    switch (event->button())
    {
        case Qt::LeftButton:
            m = QGuiApplication::keyboardModifiers();
            if (m.testFlag(Qt::ControlModifier))
                CTRL = true;
            else if (m.testFlag(Qt::ShiftModifier))
                SHIFT = true;

            if (!shapeConnected)
                appendToShape(event->pos(), CTRL, SHIFT);
            break;

        case Qt::RightButton:
            if (!cutterConnected)
                appendToCutter(event->pos());
            break;

        case Qt::MiddleButton:
            if (!shapeConnected)
                connectShape();
            break;

        case Qt::BackButton:
            if (!cutterConnected)
                connectCutter();
            break;

        default:
            break;
    }

    viewport()->update();
}

void Plane::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Space:
            if (!shapeConnected)
                connectShape();
            break;

        case Qt::Key_Return:
            if (!cutterConnected)
                connectCutter();
            break;

        default:
            break;
    }

    viewport()->update();
}

void Plane::addShapePoint(const point_t &point)
{
    appendToShape({ (int)point.x, (int)point.y }, false, false);
}

bool Plane::appendToShape(const QPoint &vertex, bool CTRL, bool SHIFT)
{
    point_t _vertex = { (double)vertex.x(), (double)vertex.y() };
    std::map<double, point_t> dists;
    double minDist = 1e9, dist;

    for (const auto &v : cutter.vertices)
        if (v.x == _vertex.x && v.y == _vertex.y)
            return false;

    if (!cutter.edges.empty() && CTRL)
    {
        point_t intersection;

        for (const line_t &edge : cutter.edges)
        {
            if (findIntersection(edge, _vertex, intersection))
            {
                dist = std::sqrt(std::pow(_vertex.x - intersection.x, 2)
                                 + std::pow(_vertex.y - intersection.y, 2));
                dists[dist] = intersection;
            }
        }

        // qDebug() << "original: (" << _vertex.x << ";" << _vertex.y << ")";
        for (const auto &p : dists)
        {
            // qDebug() << "dist:" << p.first << "coords:" << p.second.x << p.second.y;
            if (p.first < minDist)
            {
                minDist = p.first;
                _vertex = p.second;
            }
        }
        // qDebug() << "replaced: (" << _vertex.x << ";" << _vertex.y << ")";
    }
    else if (!cutter.vertices.empty() && SHIFT)
    {
        for (const point_t &cv : cutter.vertices)
        {
            dist = std::sqrt(std::pow(_vertex.x - cv.x, 2) + std::pow(_vertex.y - cv.y, 2));
            dists[dist] = cv;
        }

        // qDebug() << "original: (" << _vertex.x << ";" << _vertex.y << ")";
        for (const auto &p : dists)
        {
            // qDebug() << "dist:" << p.first << "coords:" << p.second.x << p.second.y;
            if (p.first < minDist)
            {
                minDist = p.first;
                _vertex = p.second;
            }
        }
        // qDebug() << "replaced: (" << _vertex.x << ";" << _vertex.y << ")";
    }

    if (shape.vertices.size() > 0)
        shape.edges.append({ shape.vertices.last(), _vertex });
    shape.vertices.append(_vertex);

    return true;
}

void Plane::addCutterPoint(const point_t &point)
{
    appendToCutter({ (int)point.x, (int)point.y });
}

bool Plane::appendToCutter(const QPoint &vertex)
{
    point_t _vertex = { (double)vertex.x(), (double)vertex.y() };

    for (const auto &v : cutter.vertices)
        if (v.x == _vertex.x && v.y == _vertex.y)
            return false;

    if (cutter.vertices.size() > 0)
        cutter.edges.append({ cutter.vertices.last(), _vertex });
    cutter.vertices.append(_vertex);

    return true;
}

bool Plane::connectShape()
{
    if (shape.vertices.size() <= 2)
    {
        QMessageBox::critical(this, "Ошибка",
                              "Невозможно замкнуть многоугольник. Многоугольник должен "
                              "содержать не менее трех точек!");
        return false;
    }

    shape.edges.append({ shape.vertices.last(), shape.vertices.first() });
    shapeConnected = true;
    return true;
}

bool Plane::connectCutter()
{
    if (cutter.vertices.size() <= 2)
    {
        QMessageBox::critical(this, "Ошибка",
                              "Невозможно замкнуть отсекатель. Отсекатель должен "
                              "содержать не менее трех точек!");
        return false;
    }

    cutter.edges.append({ cutter.vertices.last(), cutter.vertices.first() });
    cutterConnected = true;
    return true;
}

void Plane::clearPlane()
{
    shape.edges.clear();
    shape.vertices.clear();
    shapeConnected = false;

    cutter.edges.clear();
    cutter.vertices.clear();
    cutterConnected = false;

    viewport()->update();
}

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

bool Plane::findIntersection(const line_t &line, const point_t &p, point_t &intersection)
{
    double dx = line.end.x - line.start.x;
    double dy = line.end.y - line.start.y;

    if (std::abs(dx) < 1e-10 && std::abs(dy) < 1e-10)
    {
        return false;
    }

    double a = dy;
    double b = -dx;
    double c = dx * line.start.y - dy * line.start.x;

    double denom = a * (-dy) + b * dx;
    if (std::abs(denom) < 1e-10)
    {
        return false;
    }

    double t = -(a * p.x + b * p.y + c) / denom;

    intersection.x = p.x - t * dy;
    intersection.y = p.y + t * dx;

    return true;
}
