#ifndef PLANE_H
#define PLANE_H

#include <QGraphicsView>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QVector>
#include <QPen>

class Plane : public QGraphicsView
{
    Q_OBJECT

public:
    int pointsCount;

    explicit Plane(QWidget *parent = nullptr);
    QVector<std::pair<int, QPointF>> points;
    QPointF screenCoordToRealCoord(QPointF point);
    QPointF realCoordToScreenCoord(QPointF point);
    void addTriangle(std::array<QPointF, 3> trianglePoints);
    void scale();

signals:
    void clicked(QPointF event);

private:
    void drawAxis(QPainter &painter);
    void drawGrid(QPainter &painter, int span);
    void drawDashedVLine(QPainter &painter, int x, int y1, int y2, int gap, int dash_len);
    void drawDashedHLine(QPainter &painter, int y, int x1, int x2, int gap, int dash_len);
    void drawTriangle(QPainter &painter);

    std::array<QPointF, 3> triangle;
    bool triangleInitialized;
    double scaleFactor;

    double minRatioNoScale = 0.05;
    double maxRatioNoScale = 1;
    double scaleAmount = 0.2;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // PLANE_H
