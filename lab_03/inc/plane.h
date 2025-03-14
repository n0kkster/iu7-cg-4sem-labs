#ifndef PLANE_H
#define PLANE_H

#include <QGraphicsView>
#include <QPainter>
#include <QVector>
#include <QColor>

typedef enum
{
    DDA,
    BRES_REAL,
    BRES_INT,
    BRES_SMOOTH,
    WU,
    BUILTIN,
    METHOD_COUNT
} method_e;

typedef struct
{
    QPointF start, end;
    method_e method;
    QColor color;
} line_t;

class Plane : public QGraphicsView
{
    Q_OBJECT

private:

    // ======== DATA ========
    QVector<line_t> lines;
    QColor color;
    // ======================

    // ======== FUNCS ========
    void myDrawLine(QPainter &painter, const line_t &line);

    void drawAxis(QPainter &painter);
    void drawDashedVLine(QPainter &painter, int x, int y1, int y2, int gap, int dash_len);
    void drawDashedHLine(QPainter &painter, int y, int x1, int x2, int gap, int dash_len);
    void drawGrid(QPainter &painter);
    // =======================

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit Plane(QWidget *parent = nullptr);

    void addSpectre(double length, method_e method, double angle, const QColor &color);
    void addLine(line_t line);

    void setBGColor(const QColor &color);
    void clearPlane();

    void rotatePoint(double angle, QPointF &point, const QPointF &origin);
};

#endif // PLANE_H