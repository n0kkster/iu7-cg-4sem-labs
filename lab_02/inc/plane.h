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
    QPointF center;
    double a, b, angle;
} ellipse_t;

typedef struct
{
    double xmin, xmax, ymin, ymax;
} limit_t;

typedef std::vector<std::vector<double>> matrix_t;

class Plane : public QGraphicsView
{
    Q_OBJECT

public:
    explicit Plane(QWidget *parent = nullptr);
    void rollbackTransformation();
    void addOffset(offset_t offset);
    void addScale(scale_t scale);
    void addRotation(rotation_t rotation);

private:

    QPointF realCoordToScreenCoord(QPointF point);

    void drawAxis(QPainter &painter);
    void drawDashedHLine(QPainter &painter, int y, int x1, int x2, int gap, int dash_len);
    void drawDashedVLine(QPainter &painter, int x, int y1, int y2, int gap, int dash_len);
    void drawGrid(QPainter &painter);

    void drawShape(QPainter &painter);
    void drawTriangle(QPainter &painter, QPointF p1, QPointF p2, QPointF p3);
    void drawRect(QPainter &painter, QPointF p1, QPointF p2);
    void drawEllipse(QPainter &painter, const ellipse_t &ellipse, const limit_t &limit);
    void drawEllipseByPoints(QPainter &painter, QPointF center, double a, double b);

    QPointF mirrorPointByX(QPointF p, double cx);
    QPointF mirrorPointByY(QPointF p, double cy);
    bool inRange(const QPointF &p, const limit_t &limit);

    QPointF rotatePoint(const QPointF &point, const rotation_t &rotation);

    matrix_t multiplyMatrices(const matrix_t &matrix1, const matrix_t &matrix2);
    void initTMatrix(matrix_t &m);

    matrix_t curr_transformation;
    matrix_t prev_transformation;

    const int gridSpan = 50;
    int W;
    int H;

    const QPointF rectP1 = {-50, 50};
    const QPointF rectP2 = {50, -50};

    const QPointF triangleP1 = {-50, -50};
    const QPointF triangleP2 = {50, -50};
    const QPointF triangleP3 = {0, 50};

    const QPointF e1_center = {50, 0};
    const double e1_a = 30.950799;
    const double e1_b = 80.773867;
    const double angle = -atan(0.619018);

    const QPointF e2_center = {-50, 0};
    const double e2_a = e1_a;
    const double e2_b = e1_b;

    const QPointF e3_center = {0, -55};
    const double e3_a = 53.033009;
    const double e3_b = 15;


protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // PLANE_H