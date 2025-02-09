#include "plane.h"

Plane::Plane(QWidget *parent) : QGraphicsView(parent)
{
    pointsCount = 0;
    triangleInitialized = false;
    scaleFactor = 1;
}

void Plane::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(viewport());
    QPen dots_pen(Qt::yellow, 2);

    scale();

    drawGrid(painter, 50);
    drawAxis(painter);

    painter.setPen(dots_pen);
    for (const auto &point : points)
        painter.drawPoint(realCoordToScreenCoord(point.second));

    drawTriangle(painter);
}

void Plane::scale()
{
    if (!triangleInitialized)
        return;

    const double minX = std::min({triangle[0].x(), triangle[1].x(), triangle[2].x()});
    const double maxX = std::max({triangle[0].x(), triangle[1].x(), triangle[2].x()});

    const double minY = std::min({triangle[0].y(), triangle[1].y(), triangle[2].y()});
    const double maxY = std::max({triangle[0].y(), triangle[1].y(), triangle[2].y()});

    const double deltaX = maxX - minX;
    const double deltaY = maxY - minY;

    const int w = this->viewport()->width();
    const int h = this->viewport()->height();
    
    if (deltaX / w < minRatioNoScale || deltaY / h < minRatioNoScale)
        scaleFactor = std::min(w * scaleAmount / deltaX, h * scaleAmount * deltaY);
    else if (deltaX / w > maxRatioNoScale || deltaY / h > maxRatioNoScale)
        scaleFactor = std::min(w * (1 - scaleAmount) / deltaX, h * (1 - scaleAmount) * deltaY);
    else
        scaleFactor = 1;
}

void Plane::addTriangle(std::array<QPointF, 3> trianglePoints)
{
    triangle = trianglePoints;
    triangleInitialized = true;
    this->viewport()->update();
}

void Plane::drawTriangle(QPainter &painter)
{
    QPen triangle_pen(Qt::green, 2);

    if (!triangleInitialized)
        return;

    painter.setPen(triangle_pen);

    for (const auto &point : triangle)
        painter.drawPoint(realCoordToScreenCoord(point));

    painter.drawLine(realCoordToScreenCoord(triangle[0]), realCoordToScreenCoord(triangle[1]));
    painter.drawLine(realCoordToScreenCoord(triangle[1]), realCoordToScreenCoord(triangle[2]));
    painter.drawLine(realCoordToScreenCoord(triangle[2]), realCoordToScreenCoord(triangle[0]));
}

void Plane::mousePressEvent(QMouseEvent *event)
{
    QPointF point = screenCoordToRealCoord(mapToScene(event->pos()));
    points.append(std::pair<int, QPointF>{++pointsCount, point});

    emit clicked(point);

    this->viewport()->repaint();
}

// void Plane::painter.drawLine(QPainter &painter, const QPointF p1, const QPointF p2)
// {
//     painter.drawLine(painter, p1.x(), p1.y(), p2.x(), p2.y());
// }

// void Plane::painter.drawLine(QPainter &painter, double _x0, double _y0, double _x1, double _y1)
// {
//     int x0, x1, y0, y1;

//     x0 = lround(_x0);
//     x1 = lround(_x1);
//     y0 = lround(_y0);
//     y1 = lround(_y1);

//     if (x0 > x1 && y0 > y1)
//     {
//         x0 = lround(_x1);
//         x1 = lround(_x0);
//         y0 = lround(_y1);
//         y1 = lround(_y0);
//     }
    
//     int deltaX = std::abs(x1 - x0);
//     int deltaY = std::abs(y1 - y0);
//     int theta = deltaX != 0 ? deltaY / deltaX : 1;
//     int err = 0, deltaErr, p, dir;

//     qDebug() << "Theta: " << theta;

//     if (theta < 1)
//     {
//         deltaErr = deltaY + 1;
//         p = y0;
//         dir = y1 - y0 > 0 ? 1 : -1;
//         for (int x = x0; x <= x1; x++)
//         {
//             qDebug() << "X: " << x << " Y: " << p;
//             painter.drawPoint(x, p);
//             err += deltaErr;
//             if (err >= deltaX + 1)
//             {
//                 p += dir;
//                 err -= (deltaX + 1);
//             }
//         }
//     }
//     else
//     {
//         deltaErr = deltaX + 1;
//         p = x0;
//         dir = x1 - x0 > 0 ? 1 : -1;
//         for (int y = y0; y <= y1; y++)
//         {
//             qDebug() << "X: " << p << " Y: " << y;
//             painter.drawPoint(p, y);
//             err += deltaErr;
//             if (err >= deltaY + 1)
//             {
//                 p += dir;
//                 err -= (deltaY + 1);
//             }
//         }
//     }
// }

void Plane::drawGrid(QPainter &painter, int span)
{
    QPen line_pen(Qt::gray, 1);
    painter.setPen(line_pen);

    for (int x = span - 1; x < this->viewport()->size().width(); x += span)
        drawDashedVLine(painter, x, 0, this->viewport()->size().height(), 5, 10);
    for (int y = span - 1; y < this->viewport()->size().height(); y += span)
        drawDashedHLine(painter, y, 0, this->viewport()->size().width(), 5, 10);
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

    const int axis_length = 400;
    int center_x = this->viewport()->size().width() / 2;
    int center_y = this->viewport()->size().height() / 2;

    painter.drawLine(center_x - axis_length / 2, center_y, center_x + axis_length / 2, center_y);
    painter.drawLine(center_x, center_y - axis_length / 2, center_x, center_y + axis_length / 2);
}

QPointF Plane::screenCoordToRealCoord(QPointF point)
{
    return QPointF{point.rx() / scaleFactor, -point.ry() / scaleFactor};
}

QPointF Plane::realCoordToScreenCoord(QPointF point)
{
    return QPointF{point.rx() * scaleFactor + this->viewport()->size().width() / 2,
                    -point.ry() * scaleFactor + this->viewport()->size().height() / 2};
}
