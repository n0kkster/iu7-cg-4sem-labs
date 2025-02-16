#include "plane.h"

// Конструктор
// ==================================================
Plane::Plane(QWidget *parent) : QGraphicsView(parent) 
{

}
// ==================================================


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

void Plane::addTransformation(offset_t offset, rotation_t rotation, scale_t scale)
{
    transform_t t = {offset, scale, rotation};
    transformations.push_back(t);
}

void Plane::rollbackTransformation()
{   
        if (transformations.size() > 0)
        transformations.pop_back();
    this->viewport()->update();
}


void Plane::applyTransform(QPointF &point, const transform_t &transform)
{
    offsetPoint(point, transform.offset);
    rotatePoint(point, transform.rotation);
    scalePoint(point, transform.scale);
}

void Plane::offsetPoint(QPointF &point, const offset_t &offset)
{
    point = {point.x() + offset.x, point.y() + offset.y};
}

void Plane::rotatePoint(QPointF &point, const rotation_t &rotation)
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

void Plane::scalePoint(QPointF &point, const scale_t &scale)
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
    // transformPoint(point);
    for (const auto &t : transformations)
        applyTransform(point, t);    
    return {point.rx() + W / 2, -point.ry() + H / 2};
}

