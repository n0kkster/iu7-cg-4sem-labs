#ifndef PLANE_H
#define PLANE_H

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
    QVector<std::pair<QPointF, double>> circles; 
    // std::pair<QPointF, double> answerCircle;
    std::pair<std::pair<QPointF, double>, QPointF> answer;

    QPointF screenCoordToRealCoord(QPointF point);
    QPointF realCoordToScreenCoord(QPointF point);

    int pointsCount;

    void addTriangle(std::array<QPointF, 3> trianglePoints);
    void scale();
    bool circleInVector(QPointF center);
    void resetAnswerState() { answerFound = false; }

public slots:
    void onSolveBtnClicked();

signals:
    void clicked(QPointF event);

private:
    void drawAxis(QPainter &painter);
    void drawGrid(QPainter &painter, int span);
    void drawDashedVLine(QPainter &painter, int x, int y1, int y2, int gap, int dash_len);
    void drawDashedHLine(QPainter &painter, int y, int x1, int x2, int gap, int dash_len);
    void drawTriangle(QPainter &painter);

    bool arePointsOnSameLine(QPointF p1, QPointF p2, QPointF p3);
    QPointF calcCircleCenter(QPointF p1, QPointF p2, QPointF p3);
    double calcDistance(QPointF p1, QPointF p2);
    double calcAngle(QPointF p1, QPointF p2, QPointF p3, QPointF p4);
    double radToDeg(double radians);
    double degToRad(double angle);
    void drawAnswer(QPainter &painter);

    // void drawLine(QPainter &painter, double x1, double y1, double x2, double y2);
    // void drawLine(QPainter &painter, const QPointF p1, const QPointF p2);

    std::array<QPointF, 3> triangle;

    bool triangleInitialized;
    bool answerFound;

    const double defaultScale = 1;

    double scaleFactor;
    double minRatioNoScale = 0.05;
    double maxRatioNoScale = 1;
    double scaleAmount = 0.2;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // PLANE_H
