#ifndef PLANE_H
#define PLANE_H

#include <QGraphicsView>
#include <QPainter>
#include <QVector>
#include <QColor>
#include <QPoint>

class Plane : public QGraphicsView
{
    Q_OBJECT

private:

    // ======== DATA ========
    QVector<QPoint> vertices;
    QColor color;
    // ======================

    // ======== FUNCS ========

    void drawAxis(QPainter &painter);
    void drawGrid(QPainter &painter);
    void drawDashedVLine(QPainter &painter, int x, int y1, int y2, int gap, int dash_len);
    void drawDashedHLine(QPainter &painter, int y, int x1, int x2, int gap, int dash_len);
    // =======================

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit Plane(QWidget *parent = nullptr);
    void clearPlane();
};

#endif // PLANE_H