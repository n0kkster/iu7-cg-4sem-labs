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

    bool needCut;
    bool shapeConnected;
    bool cutterConnected;

    QColor shapeColor;
    QColor cutterColor;
    QColor resColor;

    shape_t cutter, shape;
    // ======================

    // ======== FUNCS ========
    void drawAxis(QPainter &painter);

    bool appendToShape(const QPoint &vertex, bool Z);
    bool appendToCutter(const QPoint &vertex);
    // =======================

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

public:
    explicit Plane(QWidget *parent = nullptr);
    void clearPlane();

    void addShapePoint(const point_t &point);
    void addCutterPoint(const point_t &point);

    bool connectShape();
    bool connectCutter();

    void enableCut() { needCut = true; }

    void setShapeColor(const QColor &color) { shapeColor = color; }
    void setCutterColor(const QColor &color) { cutterColor = color; }
    void setResColor(const QColor &color) { resColor = color; }

    QColor getShapeColor() { return shapeColor; }
    QColor getCutterColor() { return cutterColor; }
    QColor getResColor() { return resColor; }
};

#endif /* PLANE_H */
