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

class Plane : public QGraphicsView
{
    Q_OBJECT

public:
    explicit Plane(QWidget *parent = nullptr);

    void addScale(double cx, double cy, double kx, double ky);
    void addOffset(double dx, double dy);
    void addRotation(double cx, double cy, double angle);

private:

    QPointF realCoordToScreenCoord(QPointF point);

    void drawEllipse(QPainter &painter);
    QPointF mirrorPointByY(QPointF p);
    QPointF mirrorPointByX(QPointF p);

    void transformPoint(QPointF &point);
    void offsetPoint(QPointF &point);
    void rotatePoint(QPointF &point);
    void scalePoint(QPointF &point);

    void initOffset();
    void initScale();
    void initRotation();

    offset_t offset;
    scale_t scale;
    rotation_t rotation;



protected:
    void paintEvent(QPaintEvent *event) override;

    int W;
    int H;
};

#endif // PLANE_H