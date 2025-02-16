#ifndef PLANE_H
#define PLANE_H

#include <QGraphicsView>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QVector>
#include <QPen>
#include <QMessageBox>

typedef struct
{
    double x;
    double y;
} offset_t;

typedef struct
{
    double cx;
    double cy;
    double kx;
    double ky;
} scale_t;

typedef struct
{
    double cx;
    double cy;
    double angle;
} rotation_t;

typedef struct
{
    offset_t offset;
    scale_t scale;
    rotation_t rotation;
} transform_t;

class Plane : public QGraphicsView
{
    Q_OBJECT

public:
    explicit Plane(QWidget *parent = nullptr);
    void addTransformation(offset_t offset, rotation_t rotation, scale_t scale);
    void rollbackTransformation();

private:

    QPointF realCoordToScreenCoord(QPointF point);

    void drawAxis(QPainter &painter);
    void drawDashedHLine(QPainter &painter, int y, int x1, int x2, int gap, int dash_len);
    void drawDashedVLine(QPainter &painter, int x, int y1, int y2, int gap, int dash_len);
    void drawGrid(QPainter &painter);

    void drawTriangle(QPainter &painter, QPointF p1, QPointF p2, QPointF p3);
    void drawRect(QPainter &painter, QPointF p1, QPointF p2);
    void drawEllipse(QPainter &painter, QPointF center, double a, double b);
    void drawEllipseByPoints(QPainter &painter, QPointF center, double a, double b);

    QPointF mirrorPointByX(QPointF p, double cx);
    QPointF mirrorPointByY(QPointF p, double cy);

    void applyTransform(QPointF &point, const transform_t &transform);
    void offsetPoint(QPointF &point, const offset_t &offset);
    void rotatePoint(QPointF &point, const rotation_t &rotation);
    void scalePoint(QPointF &point, const scale_t &scale);

    std::vector<transform_t> transformations;

    const int gridSpan = 50;
    int W;
    int H;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // PLANE_H