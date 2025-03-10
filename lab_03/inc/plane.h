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
    BRES,
    WU,
    BUILTIN
} method_e;

typedef struct
{
    QPointF start, end;
    method_e method;
    QColor color;
} line_t;

#include "dda.h"

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
    // =======================

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit Plane(QWidget *parent = nullptr);

    void addLine(line_t line);
    void setBGColor(const QColor &color);
};

#endif // PLANE_H