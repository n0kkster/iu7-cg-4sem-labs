#ifndef PLANE_H
#define PLANE_H

#include "common.h"

#include <map>
#include <QColor>
#include <QGraphicsView>
#include <QPainter>
#include <QPoint>
#include <QVector>

class Plane : public QGraphicsView
{
    Q_OBJECT

private:
    // ======== DATA ========
    bool lineEnterStarted;
    QPoint line_start;

    QPoint rect_start;

    QColor lineColor;
    QColor rectColor;
    QColor resColor;

    QRect rect;
    QVector<line_t> lines;
    QVector<line_t> res_lines;
    // ======================

    // ======== FUNCS ========
    void addLine(const line_t &line);
    void addRect(const QRect &_rect);

    void drawAxis(QPainter &painter);
    // =======================

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

public:
    explicit Plane(QWidget *parent = nullptr);
    void clearPlane();

    void setLineColor(const QColor &color) { lineColor = color; }
    void setRectColor(const QColor &color) { rectColor = color; }
    void setResColor(const QColor &color) { resColor = color; }
};

#endif /* PLANE_H */
