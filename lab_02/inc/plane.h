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

    void drawEllipse(QPainter &painter);
    QPointF mirrorPointByY(QPointF p);
    QPointF mirrorPointByX(QPointF p);

    void applyTransform(QPointF &point, const transform_t &transform);
    void offsetPoint(QPointF &point, const offset_t &offset);
    void rotatePoint(QPointF &point, const rotation_t &rotation);
    void scalePoint(QPointF &point, const scale_t &scale);

    std::vector<transform_t> transformations;


protected:
    void paintEvent(QPaintEvent *event) override;

    int W;
    int H;
};

#endif // PLANE_H