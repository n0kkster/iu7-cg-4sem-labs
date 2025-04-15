#ifndef PLANE_H
#define PLANE_H

#include <QGraphicsView>
#include <QPainter>
#include <QVector>
#include <QColor>
#include <QPoint>
#include <map>

#include "common.h"

class Plane : public QGraphicsView
{
    Q_OBJECT

private:

    // ======== DATA ========
    QVector<shape_t> shapes;
    QColor fillColor;
    std::map<int, std::vector<point_t>> outline_points;

    bool fillEnabled;
    bool delayEnabled;
    int stop_line;
    // ======================

    // ======== FUNCS ========
    bool appendToShape(QVector<point_t> &points, QVector<edge_t> &edges, const QPoint &vertex, bool Z);
    bool connectShape(shape_t &shape);

    void drawAxis(QPainter &painter);
    void drawGrid(QPainter &painter);
    void drawDashedVLine(QPainter &painter, int x, int y1, int y2, int gap, int dash_len);
    void drawDashedHLine(QPainter &painter, int y, int x1, int x2, int gap, int dash_len);
    // =======================

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked(const QPoint &pos);
    void shapeFinished();

public:
    explicit Plane(QWidget *parent = nullptr);
    void clearPlane();
    bool finishShapeEntering();
    bool addVertex(const QPoint &vertex, bool Z = false);
    void fillSlowed();

    void resetShapes();

    void enableFill() { fillEnabled = true; }
    void enableDelay() { delayEnabled = true; resetShapes(); outline_points.clear(); } 
    void disableDelay() { delayEnabled = false; resetShapes(); outline_points.clear(); fillEnabled = false; }
    bool isDelayEnabled() const { return delayEnabled; } 

    int getTotalPointsCount() const;
};

#endif /* PLANE_H */
