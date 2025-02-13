#ifndef PLANE_H
#define PLANE_H

#include <sstream>

#include <QGraphicsView>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QVector>
#include <QPen>
#include <QMessageBox>

class Plane : public QGraphicsView
{
    Q_OBJECT

public:
    explicit Plane(QWidget *parent = nullptr);
    
    QVector<std::pair<int, QPointF>> points;
    std::tuple<QPointF, double, QPointF> answer;
    std::array<QPointF, 3> circlePoints;

    QPointF screenCoordToRealCoord(QPointF point);
    QPointF realCoordToScreenCoord(QPointF point);

    int pointsCount, id;

    void addTriangle(std::array<QPointF, 3> trianglePoints);
    void resetAnswerState() { answerFound = false; offset = {0, 0}; }
    bool getAnswerFound() { return answerFound; }

public slots:
    void onSolveBtnClicked();

signals:
    void clicked(QPointF event);

private:
    void drawAxis(QPainter &painter);
    void drawGrid(QPainter &painter);
    void drawDashedVLine(QPainter &painter, int x, int y1, int y2, int gap, int dash_len);
    void drawDashedHLine(QPainter &painter, int y, int x1, int x2, int gap, int dash_len);
    void drawTriangle(QPainter &painter);
    void drawAnswer(QPainter &painter);

    void scale();
    void scaleByAnswer();
    void scaleByPoints();
    void calcScaleFactorByDelta(double deltaX, double deltaY, double threshold, double amount);


    bool arePointsOnSameLine(QPointF p1, QPointF p2, QPointF p3);
    QPointF calcCircleCenter(QPointF p1, QPointF p2, QPointF p3);
    double calcDistance(QPointF p1, QPointF p2);
    double calcAngle(QPointF p1, QPointF p2, QPointF p3, QPointF p4);
    double radToDeg(double radians);
    double degToRad(double angle);
    double checkCircle(std::pair<QPointF, double> circle, double minAngle);

    std::array<QPointF, 3> triangle;

    bool triangleInitialized;
    bool answerFound;

    const double defaultScale = 1;
    const double minScale = 1e-6, maxScale = 1e6;

    double scaleFactor;
    const double minRatioNoScale = 0.05;
    const double maxRatioNoScale = 1;
    const double scaleAmount = 0.2;
    const double eps = 1e-9;
    QPointF offset;

    const int gridSpan = 50;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // PLANE_H
