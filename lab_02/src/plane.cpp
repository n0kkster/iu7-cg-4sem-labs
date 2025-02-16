#include "plane.h"

// Конструктор
// ==================================================
Plane::Plane(QWidget *parent) : QGraphicsView(parent) 
{
    initTMatrix(curr_transformation);
}
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
    drawShape(painter);
}

// Рисовальщики
// ==================================================
void Plane::drawShape(QPainter &painter)
{
    ellipse_t e1, e2, e3;
    limit_t limit;

    drawRect(painter, rectP1, rectP2);
    drawTriangle(painter, triangleP1, triangleP2, triangleP3);

    limit.xmin = rectP1.x();
    limit.xmax = rectP2.x();
    limit.ymin = rectP2.y();
    limit.ymax = rectP1.y();

    e1.center = e1_center;
    e1.a = e1_a;
    e1.b = e1_b;
    e1.angle = angle;

    e2.center = e2_center;
    e2.a = e2_a;
    e2.b = e2_b;
    e2.angle = -angle;

    e3.center = e3_center;
    e3.a = e3_a;
    e3.b = e3_b;
    e3.angle = 0;

    drawEllipse(painter, e1, limit);
    drawEllipse(painter, e2, limit);
    drawEllipse(painter, e3, limit);
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

void Plane::drawEllipse(QPainter &painter, const ellipse_t &ellipse, const limit_t &limit)
{
    double cx = ellipse.center.x(), cy = ellipse.center.y();
    double a = ellipse.a, b = ellipse.b;
    double angle = ellipse.angle;

    double x = -a + cx, y;
    double t = 1 - std::pow((x - cx) / a, 2);
    QPointF curr_point, prev_point;
    QPointF start, end;
    rotation_t rot = {cx, cy, angle};

    if (std::abs(t) < 1e-9)
        y = cy;
    else
        y = b * std::sqrt(1 - std::pow((x - cx) / a, 2)) + cy;

    
    curr_point = {x, y};
    while (x <= cx)
    {
        prev_point = curr_point;
        
        x++;
        y = b * std::sqrt(1 - std::pow((x - cx) / a, 2)) + cy;

        curr_point = {x, y};

        start = rotatePoint(prev_point, rot);
        end = rotatePoint(curr_point, rot);

        if (inRange(start, limit) && inRange(end, limit))
            painter.drawLine(realCoordToScreenCoord(start), realCoordToScreenCoord(end));

        start = rotatePoint(mirrorPointByX(prev_point, cx), rot);
        end = rotatePoint(mirrorPointByX(curr_point, cx), rot);

        if (inRange(start, limit) && inRange(end, limit))
            painter.drawLine(realCoordToScreenCoord(start), realCoordToScreenCoord(end));

        start = rotatePoint(mirrorPointByY(prev_point, cy), rot);
        end = rotatePoint(mirrorPointByY(curr_point, cy), rot);

        if (inRange(start, limit) && inRange(end, limit))
            painter.drawLine(realCoordToScreenCoord(start), realCoordToScreenCoord(end));

        start = rotatePoint(mirrorPointByX(mirrorPointByY(prev_point, cy), cx), rot);
        end = rotatePoint(mirrorPointByX(mirrorPointByY(curr_point, cy), cx), rot);

        if (inRange(start, limit) && inRange(end, limit))
            painter.drawLine(realCoordToScreenCoord(start), realCoordToScreenCoord(end));
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

void Plane::rollbackTransformation()
{   
    curr_transformation = prev_transformation;
    this->viewport()->update();
}

QPointF Plane::rotatePoint(const QPointF &point, const rotation_t &rotation)
{
    double x = point.x(), y = point.y();
    double cx = rotation.cx, cy = rotation.cy;
    double radians = rotation.angle;

    QPointF newPoint = {
        cx + (x - cx) * cos(radians) + (y - cy) * sin(radians),
        cy + (y - cy) * cos(radians) - (x - cx) * sin(radians)
    };
    return newPoint;
}

bool Plane::inRange(const QPointF &p, const limit_t &limit)
{
    return limit.xmin <= p.x() && p.x() <= limit.xmax &&
            limit.ymin <= p.y() && p.y() <= limit.ymax; 
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
    matrix_t p;

    p.push_back({point.x()});
    p.push_back({point.y()});
    p.push_back({1});
    p = multiplyMatrices(curr_transformation, p);

    return {p[0][0] + W / 2, -p[1][0] + H / 2};
}

matrix_t Plane::multiplyMatrices(const matrix_t &matrix1, const matrix_t &matrix2) 
{
    matrix_t res;
    res.push_back({0, 0, 0});
    res.push_back({0, 0, 0});
    res.push_back({0, 0, 0});

    if (matrix1[0].size() != matrix2.size())
    {
        qDebug() << "matrix invalid dimensions:" << matrix1[0].size() << "; " << matrix2.size();
        abort();
    }

    for (size_t i = 0; i < matrix1.size(); ++i)
        for (size_t j = 0; j < matrix2[0].size(); ++j)
            for (size_t k = 0; k < matrix1[0].size(); ++k)
                res[i][j] += matrix1[i][k] * matrix2[k][j];

    return res;
}

void Plane::initTMatrix(matrix_t &m)
{
    m.clear();
    m.push_back({1, 0, 0});
    m.push_back({0, 1, 0});
    m.push_back({0, 0, 1});
}

void Plane::addOffset(offset_t offset)
{
    matrix_t offsetM;
    prev_transformation = curr_transformation;

    initTMatrix(offsetM);
    offsetM[0][2] = offset.x;
    offsetM[1][2] = offset.y;
    
    curr_transformation = multiplyMatrices(offsetM, curr_transformation);
}

void Plane::addScale(scale_t scale)
{
    matrix_t scaleM, offsetM;
    prev_transformation = curr_transformation;

    initTMatrix(scaleM);
    initTMatrix(offsetM);

    offsetM[0][2] = -scale.cx;
    offsetM[1][2] = -scale.cy;

    curr_transformation = multiplyMatrices(offsetM, curr_transformation);
    
    scaleM[0][0] = scale.kx;
    scaleM[1][1] = scale.ky;

    curr_transformation = multiplyMatrices(scaleM, curr_transformation);

    offsetM[0][2] = scale.cx;
    offsetM[1][2] = scale.cy;

    curr_transformation = multiplyMatrices(offsetM, curr_transformation);
}

void Plane::addRotation(rotation_t rotation)
{
    matrix_t rotationM, offsetM;
    double c = cos(rotation.angle), s = sin(rotation.angle);
    prev_transformation = curr_transformation;

    initTMatrix(rotationM);
    initTMatrix(offsetM);

    offsetM[0][2] = -rotation.cx;
    offsetM[1][2] = -rotation.cy;

    curr_transformation = multiplyMatrices(offsetM, curr_transformation);
    
    rotationM[0][0] = c;
    rotationM[0][1] = -s;
    rotationM[1][0] = s;
    rotationM[1][1] = c;

    curr_transformation = multiplyMatrices(rotationM, curr_transformation);

    offsetM[0][2] = rotation.cx;
    offsetM[1][2] = rotation.cy;

    curr_transformation = multiplyMatrices(offsetM, curr_transformation);
}
