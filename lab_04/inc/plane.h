#ifndef PLANE_H
#define PLANE_H

#include <QGraphicsView>
#include <QPainter>
#include <QVector>
#include <QColor>

typedef enum
{
    CANONICAL,
    PARAMETRIC,
    BRES,
    MIDPOINT,
    BUILTIN,
    METHOD_COUNT
} method_e;

typedef struct
{
    QPointF center;
    double radius;
    method_e method;
    QColor color;
} circle_t;

typedef struct
{
    QPointF center;
    double rx, ry;
    method_e method;
    QColor color;
} ellipse_t;

class Plane : public QGraphicsView
{
    Q_OBJECT

private:

    // ======== DATA ========
    QVector<circle_t> circles;
    QVector<ellipse_t> ellipses;
    QColor color;
    // ======================

    // ======== FUNCS ========
    void myDrawCircle(QPainter &painter, const circle_t &line);
    void myDrawEllipse(QPainter &painter, const ellipse_t &line);

    void drawAxis(QPainter &painter);
    void drawDashedVLine(QPainter &painter, int x, int y1, int y2, int gap, int dash_len);
    void drawDashedHLine(QPainter &painter, int y, int x1, int x2, int gap, int dash_len);
    void drawGrid(QPainter &painter);
    // =======================

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit Plane(QWidget *parent = nullptr);

    void addSpectreCircle(circle_t circle, double step, int cnt);
    void addSpectreEllipse(ellipse_t ellipse, double step, int cnt);

    void addCircle(circle_t circle);
    void addEllipse(ellipse_t ellipse);

    void setBGColor(const QColor &color);
    void clearPlane();
};

#endif // PLANE_H