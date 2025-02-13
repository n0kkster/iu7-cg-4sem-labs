#include "plane.h"

Plane::Plane(QWidget *parent) : QGraphicsView(parent)
{
    pointsCount = 0;
    id = 0;
    triangleInitialized = false;
    scaleFactor = defaultScale;
    answerFound = false;
    offset = {25, 25};
    // offset = {0, 0};
}

void Plane::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(viewport());
    QPen dots_pen(Qt::yellow, 4);

    scale();
    scaleByPoints();

    if (answerFound)
        scaleByAnswer();

    drawGrid(painter, gridSpan);
    drawAxis(painter);

    painter.setPen(dots_pen);
    if (!answerFound)
    {
        for (const auto &point : points)
            painter.drawPoint(realCoordToScreenCoord(point.second));
    }
    else
    {
        // scaleByAnswer();
        for (const auto &point : circlePoints)
        {
            qDebug() << point;
            painter.drawPoint(realCoordToScreenCoord(point));
        }
    }
    

    drawTriangle(painter);

    if (answerFound)
        drawAnswer(painter);
}

void Plane::onSolveBtnClicked()
{
    QPointF p1, p2, p3, center;
    double radius, minAngle = 90, temp;

    if (!triangleInitialized)
    {
        QMessageBox::critical(this, "Ошибка", "Сначала необходимо построить треугольник!");
        return;
    }

    for (const auto &pair1 : points)
    {
        for (const auto &pair2 : points)
        {
            for (const auto &pair3 : points)
            {
                p1 = pair1.second;
                p2 = pair2.second;
                p3 = pair3.second;

                if (p1 == p2 || p2 == p3 || p1 == p3 || arePointsOnSameLine(p1, p2, p3))
                    continue;

                center = calcCircleCenter(p1, p2, p3);
                radius = calcDistance(center, p1);
                
                if (!(arePointsOnSameLine(triangle[0], triangle[1], center) ||
                    arePointsOnSameLine(triangle[1], triangle[2], center) ||
                    arePointsOnSameLine(triangle[0], triangle[2], center)))
                    continue;

                temp = checkCircle({center, radius}, minAngle);
                if (temp < minAngle)
                {
                    minAngle = temp;
                    circlePoints[0] = p1;
                    circlePoints[1] = p2;
                    circlePoints[2] = p3;
                }
            }
        }
    }

    this->viewport()->update();

    std::stringstream info_answer;
    auto [ansCenter, ansRadius, trianglePoint] = answer;
    double cx = ansCenter.x(), cy = ansCenter.y();

    if (!answerFound)
        info_answer << "Подходящие окружности не найдены.";
    else
    {
        info_answer << "Окружность найдена! Центр окружности: (" << cx << ", " << cy << ")\n";
        info_answer << "Радиус: " << ansRadius << std::endl;
        info_answer << "Прямая, проходящая через точки: (" << cx << ", " << cy << ")";
        info_answer << " и (" << trianglePoint.x() << ", " << trianglePoint.y() << "),\nсодержит сторону треугольника и образует угол " << minAngle << " градусов с осью ординат.";
    }

    QMessageBox::information(this, "Ответ", QString(info_answer.str().c_str()));
}

double Plane::checkCircle(std::pair<QPointF, double> circle, double minAngle)
{
    double angle;
    QPointF p1, p2;

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
        return 90;

    angle = calcAngle({0, 0}, {0, 1}, p1, p2); 

    if (angle <= minAngle)
    {
        minAngle = angle;
        answer = {circle.first, circle.second, p1};
        answerFound = true;
    }

    return minAngle;
}

void Plane::drawAnswer(QPainter &painter)
{
    QPen circles_pen(Qt::red, 2);
    auto [center, radius, trianglePoint] = answer;
    double cx = center.x(), cy = center.y();

    const double x1 = cx, y1 = cy, x2 = trianglePoint.x(), y2 = trianglePoint.y();
    const double x = 0;
    double y = ((y2 - y1) / (x2 - x1)) * (x - x1) + y1;

    painter.setPen(circles_pen);
    painter.drawEllipse(realCoordToScreenCoord(center), radius * scaleFactor, radius * scaleFactor);
    painter.drawPoint(realCoordToScreenCoord(center));
    painter.drawLine(realCoordToScreenCoord(center), realCoordToScreenCoord({x, y}));
    painter.drawLine(realCoordToScreenCoord(trianglePoint), realCoordToScreenCoord({x, y}));
}

double Plane::calcAngle(QPointF p1, QPointF p2, QPointF p3, QPointF p4)
{
    QPointF v1{p2.x() - p1.x(), p2.y() - p1.y()}, v2{p4.x() - p3.x(), p4.y() - p3.y()};

    double cosine = std::abs(v1.x() * v2.x() + v1.y() * v2.y());

    if (std::abs(cosine) > 1e-9)
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
    return std::abs((p2.x() - p1.x()) * (p3.y() - p1.y()) - (p3.x() - p1.x()) * (p2.y() - p1.y())) < eps;
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
        scaleFactor = std::min(w * scaleAmount / deltaX, h * scaleAmount / deltaY);
    else if (deltaX / w > maxRatioNoScale || deltaY / h > maxRatioNoScale)
        scaleFactor = std::min(w * (1 - scaleAmount) / deltaX, h * (1 - scaleAmount) / deltaY);
    else
        scaleFactor = defaultScale;
}

void Plane::scaleByPoints()
{
    double temp;
    const int w = this->viewport()->width();
    const int h = this->viewport()->height();

    const int cx = (w / 2 - offset.x()) / scaleFactor;
    const int cy = (h / 2 + offset.y()) / scaleFactor;

    // qDebug() << "============================";

    // qDebug() << "cx:" << cx << "cy:" << cy;

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
    
    if (std::max(std::abs(minX), std::abs(minY)) > 20 / scaleFactor && std::abs(maxX) < std::abs(cx) && std::abs(maxY) < std::abs(cx))
        return;

    minX = std::abs(minX);
    maxX = std::abs(maxX);

    minY = std::abs(minY);
    maxY = std::abs(maxY);

    double deltaX = std::max(maxX, minX);
    double deltaY = std::max(maxY, minY);

    // qDebug() << "deltaX: " << deltaX << " deltaY: " << deltaY;

    if (deltaX < cx && deltaY < cy && std::min(deltaX, deltaY) > 20 / scaleFactor)
        return;

    temp = std::min(cx / deltaX, cy / deltaY) * scaleFactor;

    if (temp > maxScale || temp < minScale || std::abs(temp - scaleFactor) < 1e-6)
        return;

    scaleFactor = temp / 1.1;

    // qDebug() << "scale: " << scaleFactor;
}

void Plane::scaleByAnswer()
{
    double circleMaxX, circleMinX, circleMaxY, circleMinY;
    auto [center, radius, trianglePoint] = answer;

    QPointF line = {std::abs(trianglePoint.x() - center.x()), std::abs(trianglePoint.y() - center.y())};

    double cx = center.x(), cy = center.y();

    circleMaxX = cx + radius;
    circleMinX = cx - radius;

    circleMaxY = cy + radius;
    circleMinY = cy - radius;

    const double minX = std::min({triangle[0].x(), triangle[1].x(), triangle[2].x(), circleMinX});
    const double maxX = std::max({triangle[0].x(), triangle[1].x(), triangle[2].x(), circleMaxX});

    double minY = std::min({triangle[0].y(), triangle[1].y(), triangle[2].y(), circleMinY});
    double maxY = std::max({triangle[0].y(), triangle[1].y(), triangle[2].y(), circleMaxY});

    // qDebug() << "minX: " << minX << " maxX: " << maxX << " minY: " << minY << " maxY: " << maxY;
    
    const double deltaX = maxX - minX;
    const double deltaY = maxY - minY;

    // qDebug() << "deltaX: " << deltaX << " deltaY: " << deltaY;

    const int w = this->viewport()->width();
    const int h = this->viewport()->height();

    // qDebug() << "w:" << w << "h:" << h;
    
    if (deltaX / w < 0.9 || deltaY / h < 0.9)
        scaleFactor = std::min(w * 0.9 / deltaX, h * 0.9 / deltaY);
    else if (deltaX / w > maxRatioNoScale || deltaY / h > maxRatioNoScale)
        scaleFactor = std::min(w * (1 - scaleAmount) / deltaX, h * (1 - scaleAmount) / deltaY);
    else
        scaleFactor = defaultScale;

    scaleFactor /= 1.4;

    const double xOffset = std::min(trianglePoint.x(), center.x()) + line.x() / 2;
    const double yOffset = std::min(trianglePoint.y(), center.y()) + line.y() / 2;

    // qDebug() << "xOffset:" << xOffset << "yOffset:" << yOffset;

    offset = {-xOffset * scaleFactor, -yOffset * scaleFactor};
    
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
    QPointF point = screenCoordToRealCoord(event->pos());
    points.append(std::pair<int, QPointF>{++id, point});
    ++pointsCount;

    if (answerFound)
        resetAnswerState();

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

    for (int x = -1; x < this->viewport()->size().width(); x += span)
        drawDashedVLine(painter, x + offset.x() , 0, this->viewport()->size().height(), 5, 10);
    for (int y = -1; y < this->viewport()->size().height(); y += span)
        drawDashedHLine(painter, y + offset.y(), 0, this->viewport()->size().width(), 5, 10);
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
    
    int center_x = w / 2;
    int center_y = h / 2;

    painter.drawLine(0, center_y - offset.y(), w, center_y - offset.y());
    painter.drawLine(center_x + offset.x(), 0, center_x + offset.x(), h);

    painter.setPen(QPen{Qt::white, 3});

    for (int x = center_x + offset.x() + gridSpan; x < w; x += gridSpan)
    {
        painter.drawLine(x, center_y - 5 - offset.y(), x, center_y + 5 - offset.y());
        painter.drawText(x + 5, center_y + 20 - offset.y(), QString::number((x - center_x - offset.x()) / scaleFactor, 'f', 2));
    }

    for (int x = center_x + offset.x() - gridSpan; x > 0; x -= gridSpan)
    {
        painter.drawLine(x, center_y - 5 - offset.y(), x, center_y + 5 - offset.y());
        painter.drawText(x + 5, center_y + 20 - offset.y(), QString::number((x - center_x - offset.x()) / scaleFactor, 'f', 2));
    }

    for (int y = center_y - offset.y() + gridSpan; y < h; y += gridSpan)
    {
        painter.drawLine(center_x - 5 + offset.x(), y, center_x + 5 + offset.x(), y);
        painter.drawText(center_x + 5 + offset.x(), y + 20, QString::number((center_y - y - offset.y()) / scaleFactor, 'f', 2));
    }
    
    for (int y = center_y - offset.y() - gridSpan; y > 0; y -= gridSpan)
    {
        painter.drawLine(center_x - 5 + offset.x(), y, center_x + 5 + offset.x(), y);
        painter.drawText(center_x + 5 + offset.x(), y + 20, QString::number((center_y - y - offset.y()) / scaleFactor, 'f', 2));
    }
}

QPointF Plane::screenCoordToRealCoord(QPointF point)
{
    return QPointF{(point.rx() - offset.x() - this->viewport()->size().width() / 2) / scaleFactor, 
        (-point.ry() - offset.y() + this->viewport()->size().height() / 2) / scaleFactor};
}

QPointF Plane::realCoordToScreenCoord(QPointF point)
{
    return QPointF{(point.rx() * scaleFactor) + offset.x() + this->viewport()->size().width() / 2,
                    (-point.ry() * scaleFactor) - offset.y() + this->viewport()->size().height() / 2};
}
