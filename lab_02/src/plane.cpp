#include "plane.h"

// Конструктор
// ==================================================
Plane::Plane(QWidget *parent) : QGraphicsView(parent) {}
// ==================================================


// ==================================================
void Plane::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(viewport());

    W = this->viewport()->size().width();
    H = this->viewport()->size().height();
    
    drawGrid(painter);
    drawAxis(painter);

    painter.setPen({Qt::yellow, 1});
}


void Plane::drawTriangle(QPainter &painter, QPointF p1, QPointF p2, QPointF p3)
{
    painter.drawLine(realCoordToScreenCoord(p1), realCoordToScreenCoord(p2));
    painter.drawLine(realCoordToScreenCoord(p1), realCoordToScreenCoord(p3));
    painter.drawLine(realCoordToScreenCoord(p2), realCoordToScreenCoord(p3));
}

void Plane::drawRect(QPainter &painter, QPointF p1, QPointF p2)
{
    double x1 = p1.x(), x2 = p2.x(), y1 = p1.y(), y2 = p2.y();
    painter.drawLine(realCoordToScreenCoord({x1, y1}), realCoordToScreenCoord({x2, y1}));
    painter.drawLine(realCoordToScreenCoord({x1, y1}), realCoordToScreenCoord({x1, y2}));
    painter.drawLine(realCoordToScreenCoord({x1, y2}), realCoordToScreenCoord({x2, y2}));
    painter.drawLine(realCoordToScreenCoord({x2, y1}), realCoordToScreenCoord({x2, y2}));
}

void Plane::drawEllipse(QPainter &painter, QPointF center, double a, double b)
{
    double x = -a + center.x(), y = b * std::sqrt(1 - std::pow((x - center.x()) / a, 2)) + center.y();
    QPointF curr_point, prev_point;

    curr_point = {x, y};
    while (x < center.x())
    {
        prev_point = curr_point;
        
        x++;
        y = b * std::sqrt(1 - std::pow((x - center.x()) / a, 2)) + center.y();

        curr_point = {x, y};

        painter.drawLine(realCoordToScreenCoord(prev_point), 
            realCoordToScreenCoord(curr_point));

        painter.drawLine(realCoordToScreenCoord(mirrorPointByX(prev_point, center.x())), 
            realCoordToScreenCoord(mirrorPointByX(curr_point, center.x())));

        painter.drawLine(realCoordToScreenCoord(mirrorPointByY(prev_point, center.y())), 
            realCoordToScreenCoord(mirrorPointByY(curr_point, center.y())));

        painter.drawLine(realCoordToScreenCoord(mirrorPointByX(mirrorPointByY(prev_point, center.y()), center.x())), 
            realCoordToScreenCoord(mirrorPointByX(mirrorPointByY(curr_point, center.y()), center.x())));
    }
}

void Plane::drawEllipseByPoints(QPainter &painter, QPointF center, double a, double b)
{
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
        painter.drawPoint(realCoordToScreenCoord(mirrorPointByX(curr_point, center.x())));
        painter.drawPoint(realCoordToScreenCoord(mirrorPointByY(curr_point, center.y())));
        painter.drawPoint(realCoordToScreenCoord(mirrorPointByX(mirrorPointByY(curr_point, center.y()), center.x())));
    }

    painter.drawPoint(realCoordToScreenCoord(next_point));
    painter.drawPoint(realCoordToScreenCoord(mirrorPointByX(next_point, center.x())));
    painter.drawPoint(realCoordToScreenCoord(mirrorPointByY(next_point, center.y())));
    painter.drawPoint(realCoordToScreenCoord(mirrorPointByX(mirrorPointByY(next_point, center.y()), center.x())));
}

void Plane::drawGrid(QPainter &painter)
{
    QPen line_pen(Qt::gray, 1);
    painter.setPen(line_pen);

    for (int x = W / 2 + gridSpan; x < W; x += gridSpan)
        drawDashedVLine(painter, x, 0, H, 5, 10);

    for (int x = W / 2 - gridSpan; x > 0; x -= gridSpan)
        drawDashedVLine(painter, x, 0, H, 5, 10);

    for (int y = H / 2 + gridSpan; y < H; y += gridSpan)
        drawDashedHLine(painter, y, 0, W, 5, 10);

    for (int y = H / 2 - gridSpan; y > 0; y -= gridSpan)
        drawDashedHLine(painter, y, 0, W, 5, 10);
}

void Plane::drawDashedVLine(QPainter &painter, int x, int y1, int y2, int gap, int dash_len)
{
    for (; y1 < y2; y1 += dash_len + gap)
        painter.drawLine(x, y1, x, y1 + dash_len);
}

void Plane::drawDashedHLine(QPainter &painter, int y, int x1, int x2, int gap, int dash_len)
{
    for (; x1 < x2; x1 += dash_len + gap)
        painter.drawLine(x1, y, x1 + dash_len, y);
}

void Plane::drawAxis(QPainter &painter)
{
    QPen line_pen(Qt::gray, 3);
    painter.setPen(line_pen);

    int center_x = W / 2;
    int center_y = H / 2;

    painter.drawLine(0, center_y, W, center_y);
    painter.drawLine(center_x, 0, center_x, H);

    painter.setPen(QPen{Qt::white, 3});

    for (int x = center_x + gridSpan; x < W; x += gridSpan)
    {
        painter.drawLine(x, center_y - 5, x, center_y + 5);
        painter.drawText(x + 5, center_y + 20, QString::number(x - center_x, 'f', 2));
    }

    for (int x = center_x - gridSpan; x > 0; x -= gridSpan)
    {
        painter.drawLine(x, center_y - 5, x, center_y + 5);
        painter.drawText(x + 5, center_y + 20, QString::number(x - center_x, 'f', 2));
    }

    for (int y = center_y + gridSpan; y < H; y += gridSpan)
    {
        painter.drawLine(center_x - 5, y, center_x + 5, y);
        painter.drawText(center_x + 5, y + 20, QString::number(center_y - y, 'f', 2));
    }
    
    for (int y = center_y - gridSpan; y > 0; y -= gridSpan)
    {
        painter.drawLine(center_x - 5, y, center_x + 5, y);
        painter.drawText(center_x + 5, y + 20, QString::number(center_y - y, 'f', 2));
    }

    painter.drawText(W - 10, center_y - 5, "X");
    painter.drawText(center_x + 10, 12, "Y");
}
// ==================================================

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

QPointF Plane::mirrorPointByX(QPointF p, double cx)
{
    return {2 * cx - p.x(), p.y()};
}

QPointF Plane::mirrorPointByY(QPointF p, double cy)
{
    return {p.x(), 2 * cy - p.y()};
}

QPointF Plane::realCoordToScreenCoord(QPointF point)
{
    for (const auto &t : transformations)
        applyTransform(point, t);    
    return {point.rx() + W / 2, -point.ry() + H / 2};
}

