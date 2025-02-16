#include "plane.h"

// Конструктор
// ==================================================
Plane::Plane(QWidget *parent) : QGraphicsView(parent) 
{
    initOffset();
    initScale();
    initRotation();
}
// ==================================================

void Plane::initOffset()
{
    offset.x = 0;
    offset.y = 0;
}

void Plane::initScale()
{
    scale.cx = 0;
    scale.cy = 0;
    scale.kx = 1;
    scale.ky = 1;
}

void Plane::initRotation()
{
    rotation.angle = 0;
    rotation.cx = 0;
    rotation.cy = 0;
}

void Plane::addScale(double cx, double cy, double kx, double ky)
{
    scale.cx = cx;
    scale.cy = cy;
    scale.kx = kx;
    scale.ky = ky;
}

void Plane::addOffset(double dx, double dy)
{
    offset.x += dx;
    offset.y += dy;
}

void Plane::addRotation(double cx, double cy, double angle)
{
    rotation.cx = cx;
    rotation.cy = cy;
    rotation.angle += angle;
}


// ==================================================
void Plane::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(viewport());

    W = this->viewport()->size().width();
    H = this->viewport()->size().height();
    
    painter.setPen({Qt::yellow, 1});
    drawEllipse(painter);
}

void Plane::drawEllipse(QPainter &painter)
{
    double a = 50.0, b = 25.0;
    double rx = -a, ry = 0, dx, dy;
    QPointF curr_point, next_point;

    while (rx < 0)
    {
        curr_point = {rx, ry};
        next_point = {rx + 1, b * std::sqrt(1 - std::pow((rx + 1) / a, 2))};

        dx = next_point.x() - curr_point.x();
        dy = next_point.y() - curr_point.y();

        if (dx / dy < 1)
            ry += 1;
        else
        {
            rx += 1;
            ry = b * std::sqrt(1 - std::pow(rx / a, 2));
        }

        painter.drawPoint(realCoordToScreenCoord(curr_point));
        painter.drawPoint(realCoordToScreenCoord(mirrorPointByY(curr_point)));
        painter.drawPoint(realCoordToScreenCoord(mirrorPointByX(curr_point)));
        painter.drawPoint(realCoordToScreenCoord(mirrorPointByX(mirrorPointByY(curr_point))));
    }

    painter.drawPoint(realCoordToScreenCoord(next_point));
    painter.drawPoint(realCoordToScreenCoord(mirrorPointByY(next_point)));
    painter.drawPoint(realCoordToScreenCoord(mirrorPointByX(next_point)));
    painter.drawPoint(realCoordToScreenCoord(mirrorPointByX(mirrorPointByY(next_point))));
}

void Plane::transformPoint(QPointF &point)
{
    offsetPoint(point);
    rotatePoint(point);
    scalePoint(point);
}

void Plane::offsetPoint(QPointF &point)
{
    point = {point.x() + offset.x, point.y() + offset.y};
}

void Plane::rotatePoint(QPointF &point)
{
    double x = point.x(), y = point.y();
    double cx = rotation.cx, cy = rotation.cy;
    double radians = rotation.angle * M_PI / 180;

    QPointF newPoint = {
        cx + (x - cx) * cos(radians) + (y - cy) * sin(radians),
        cy + (y - cy) * cos(radians) - (x - cx) * sin(radians)
    };
    point = newPoint;
}

void Plane::scalePoint(QPointF &point)
{
    double x = point.x(), y = point.y();
    double cx = scale.cx, cy = scale.cy;
    double kx = scale.kx, ky = scale.ky;

    QPointF newPoint = {
        x * kx + (1 - kx) * cx,
        y * ky + (1 - ky) * cy
    };

    point = newPoint;
}

QPointF Plane::mirrorPointByY(QPointF p)
{
    return {p.x(), -p.y()};
}

QPointF Plane::mirrorPointByX(QPointF p)
{
    return {-p.x(), p.y()};
}

QPointF Plane::realCoordToScreenCoord(QPointF point)
{
    transformPoint(point);
    return {point.rx() + W / 2, -point.ry() + H / 2};
}

