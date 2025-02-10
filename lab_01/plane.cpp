#include "plane.h"

Plane::Plane(QWidget *parent) : QGraphicsView(parent)
{
    pointsCount = 0;
    id = 0;
    triangleInitialized = false;
    scaleFactor = defaultScale;
    answerFound = false;
}

void Plane::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(viewport());
    QPen dots_pen(Qt::yellow, 4);

    scale();
    scaleByPoints();

    drawGrid(painter, gridSpan);
    drawAxis(painter);

    painter.setPen(dots_pen);
    for (const auto &point : points)
        painter.drawPoint(realCoordToScreenCoord(point.second));

    drawTriangle(painter);

    if (answerFound)
        drawAnswer(painter);
}

void Plane::onSolveBtnClicked()
{
    QPointF p1, p2, p3, center;
    double radius, minAngle = 90, angle;

    if (!triangleInitialized)
    {
        QMessageBox::critical(this, "Ошибка", "Сначала необходимо построить треугольник!");
        return;
    }

    for (auto &pair1 : points)
    {
        for (auto &pair2 : points)
        {
            for (auto &pair3 : points)
            {
                p1 = pair1.second;
                p2 = pair2.second;
                p3 = pair3.second;

                if (p1 == p2 || p2 == p3 || p1 == p3 || arePointsOnSameLine(p1, p2, p3))
                    continue;

                center = calcCircleCenter(p1, p2, p3);
                radius = calcDistance(center, p1);

                if (circleInVector(center))
                    continue;
                
                if (!(arePointsOnSameLine(triangle[0], triangle[1], center) ||
                    arePointsOnSameLine(triangle[1], triangle[2], center) ||
                    arePointsOnSameLine(triangle[0], triangle[2], center)))
                    continue;

                circles.append(std::pair<QPointF, double>{center, radius});
            }
        }
    }

    for (const auto &circle : circles)
    {
        if (arePointsOnSameLine(triangle[0], triangle[1], circle.first))
        {
            p1 = triangle[0];
            p2 = triangle[1];
        }
        else if (arePointsOnSameLine(triangle[1], triangle[2], circle.first))
        {
            p1 = triangle[1];
            p2 = triangle[2];
        }
        else if (arePointsOnSameLine(triangle[0], triangle[2], circle.first))
        {
            p1 = triangle[0];
            p2 = triangle[2];
        }
        else
            continue;

        angle = calcAngle(QPointF{0, 0}, QPointF{0, 1}, p1, p2); 

        if (angle <= minAngle)
        {
            minAngle = angle;
            answer.first = circle;
            answer.second = p1;
            answerFound = true;
        }
    }

    this->viewport()->update();

    if (!answerFound)
        QMessageBox::information(this, "Ответ", "Подходящие окружности не найдены.");
    else
        QMessageBox::information(this, "Ответ", "Окружность найдена!");
}

void Plane::drawAnswer(QPainter &painter)
{
    QPen circles_pen(Qt::red, 2);

    const double x1 = answer.first.first.x(), y1 = answer.first.first.y(), x2 = answer.second.x(), y2 = answer.second.y();
    const double x = 0;
    double y = ((y2 - y1) / (x2 - x1)) * (x - x1) + y1;

    painter.setPen(circles_pen);
    painter.drawEllipse(realCoordToScreenCoord(answer.first.first), answer.first.second, answer.first.second);
    painter.drawPoint(realCoordToScreenCoord(answer.first.first));
    painter.drawLine(realCoordToScreenCoord(answer.first.first), realCoordToScreenCoord(QPointF{x, y}));
}

double Plane::calcAngle(QPointF p1, QPointF p2, QPointF p3, QPointF p4)
{
    QPointF v1{p2.x() - p1.x(), p2.y() - p1.y()}, v2{p4.x() - p3.x(), p4.y() - p3.y()};

    double cosine = std::abs(v1.x() * v2.x() + v1.y() * v2.y());

    if (cosine != 0)
        cosine /= (std::sqrt(v1.x() * v1.x() + v1.y() * v1.y()) * std::sqrt(v2.x() * v2.x() + v2.y() * v2.y()));

    return radToDeg(acos(cosine));
}

double Plane::radToDeg(double radians)
{
    return radians * 180 / M_PI;
}

double Plane::degToRad(double angle)
{
    return angle * M_PI / 180;
}

QPointF Plane::calcCircleCenter(QPointF A, QPointF B, QPointF C)
{
    const double ax = A.x(), ay = A.y();
    const double bx = B.x(), by = B.y();
    const double cx = C.x(), cy = C.y();

    const double D = 2 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));
    const double Ux = ((ax * ax + ay * ay) * (by - cy) + (bx * bx + by * by) * (cy - ay) + (cx * cx + cy * cy) * (ay - by)) / D;
    const double Uy = ((ax * ax + ay * ay) * (cx - bx) + (bx * bx + by * by) * (ax - cx) + (cx * cx + cy * cy) * (bx - ax)) / D;

    return QPointF{Ux, Uy};
}

double Plane::calcDistance(QPointF p1, QPointF p2)
{
    const double dx = p1.x() - p2.x();
    const double dy = p1.y() - p2.y();

    return std::sqrt(dx * dx + dy * dy);
}

bool Plane::arePointsOnSameLine(QPointF p1, QPointF p2, QPointF p3)
{
    return std::abs((p2.x() - p1.x()) * (p3.y() - p1.y()) - (p3.x() - p1.x()) * (p2.y() - p1.y())) < 1e-9;
}

bool Plane::circleInVector(QPointF center)
{
    for (const auto &circle : circles)
        if (circle.first == center) 
            return true;

    return false;
}

void Plane::scale()
{
    if (!triangleInitialized)
        return;

    double minX = std::min({triangle[0].x(), triangle[1].x(), triangle[2].x()});
    double maxX = std::max({triangle[0].x(), triangle[1].x(), triangle[2].x()});

    double minY = std::min({triangle[0].y(), triangle[1].y(), triangle[2].y()});
    double maxY = std::max({triangle[0].y(), triangle[1].y(), triangle[2].y()});

    const double deltaX = maxX - minX;
    const double deltaY = maxY - minY;

    const int w = this->viewport()->width();
    const int h = this->viewport()->height();
    
    if (deltaX / w < minRatioNoScale || deltaY / h < minRatioNoScale)
        scaleFactor = std::min(w * scaleAmount / deltaX, h * scaleAmount * deltaY);
    else if (deltaX / w > maxRatioNoScale || deltaY / h > maxRatioNoScale)
        scaleFactor = std::min(w * (1 - scaleAmount) / deltaX, h * (1 - scaleAmount) * deltaY);
    else
        scaleFactor = defaultScale;
}

void Plane::scaleByPoints()
{
    const int w = this->viewport()->width() / scaleFactor;
    const int h = this->viewport()->height() / scaleFactor;

    const int cx = w / 2, cy = h / 2;

    if (points.empty())
    {
        if (!triangleInitialized)
            scaleFactor = defaultScale;
        return;
    }

    double minX = points.at(0).second.x(), maxX = points.at(0).second.x();
    double minY = points.at(0).second.y(), maxY = points.at(0).second.y();

    for (const auto &point : points)
    {
        minX = std::min(minX, point.second.x());
        maxX = std::max(maxX, point.second.x());

        minY = std::min(minY, point.second.y());
        maxY = std::max(maxY, point.second.y());
    }

    // qDebug() << "minX: " << minX << " maxX: " << maxX << " minY: " << minY << " maxY: " << maxY;
    
    double deltaX = std::max(maxX - cx, minX - cx);
    double deltaY = std::max(maxY - cy, minY - cy);

    // qDebug() << "deltaX: " << deltaX << " deltaY: " << deltaY;

    scaleFactor = std::min(cx / (cx + deltaX), cy / (cy + deltaY)) * scaleFactor / 1.1;
    // qDebug() << "scale: " << scaleFactor;
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
    points.append(std::pair<int, QPointF>{++id, point});
    ++pointsCount;

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

    const int w = this->viewport()->size().width(), h = this->viewport()->size().height();

    const int x_axis_length = w;
    const int y_axis_length = h;
    
    int center_x = w / 2;
    int center_y = h / 2;

    painter.drawLine(center_x - x_axis_length / 2, center_y, center_x + x_axis_length / 2, center_y);
    painter.drawLine(center_x, center_y - y_axis_length / 2, center_x, center_y + y_axis_length / 2);

    painter.setPen(QPen{Qt::white, 3});

    for (int x = center_x + gridSpan; x < w; x += gridSpan)
    {
        painter.drawLine(x, center_y - 5, x, center_y + 5);
        painter.drawText(x + 5, center_y + 20, QString(std::to_string((int)((x - center_x) / scaleFactor)).c_str()));
    }
    
    for (int x = center_x - gridSpan; x > 0; x -= gridSpan)
    {
        painter.drawLine(x, center_y - 5, x, center_y + 5);
        painter.drawText(x - 25, center_y + 20, QString(std::to_string((int)((x - center_x) / scaleFactor)).c_str()));

    }

    for (int y = center_y + gridSpan; y < h; y += gridSpan)
    {
        painter.drawLine(center_x - 5, y, center_x + 5, y);
        painter.drawText(center_x + 5, y + 20, QString(std::to_string((int)((center_y - y) / scaleFactor)).c_str()));
    }
    
    for (int y = center_y - gridSpan; y > 0; y -= gridSpan)
    {
        painter.drawLine(center_x - 5, y, center_x + 5, y);
        painter.drawText(center_x + 5, y + 20, QString(std::to_string((int)((center_y - y) / scaleFactor)).c_str()));
    }

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
