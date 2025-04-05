#ifndef PLANE_H
#define PLANE_H

#include <QGraphicsView>
#include <QPainter>
#include <QVector>
#include <QColor>
#include <QPoint>

#include "common.h"

class Plane : public QGraphicsView
{
    Q_OBJECT

private:

    // ======== DATA ========
    QVector<shape_t> shapes;

    QColor fillColor;
    // ======================

    // ======== FUNCS ========
    void addVertex(const QPoint &vertex);
    void appendToShape(QVector<point_t> &points, QVector<edge_t> &edges, const QPoint &vertex);
    void connectShape(shape_t &shape);

    void drawAxis(QPainter &painter);
    void drawGrid(QPainter &painter);
    void drawDashedVLine(QPainter &painter, int x, int y1, int y2, int gap, int dash_len);
    void drawDashedHLine(QPainter &painter, int y, int x1, int x2, int gap, int dash_len);
    // =======================

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public:
    explicit Plane(QWidget *parent = nullptr);
    void clearPlane();
    void finishShapeEntering();
};

#endif // PLANE_H