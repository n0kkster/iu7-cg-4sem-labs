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
    QPointF line_start;

    bool needCut;
    bool shapeConnected;

    QColor lineColor;
    QColor rectColor;
    QColor resColor;

    QVector<line_t> lines;
    shape_t cutter;
    // ======================

    // ======== FUNCS ========
    void drawAxis(QPainter &painter);

    bool appendToShape(const QPoint &vertex, bool Z);
    // =======================

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public:
    explicit Plane(QWidget *parent = nullptr);
    void clearPlane();

    void addLine(const line_t &line);
    void addShapePoint(const point_t &point);
    bool connectShape();
    void addParallelLines(int offset);

    void enableCut() { needCut = true; }

    void setLineColor(const QColor &color) { lineColor = color; }
    void setRectColor(const QColor &color) { rectColor = color; }
    void setResColor(const QColor &color) { resColor = color; }

    QColor getLineColor() { return lineColor; }
    QColor getRectColor() { return rectColor; }
    QColor getResColor() { return resColor; }
};

#endif /* PLANE_H */
