#include "cut.h"

#include "common.h"
#include "line.h"

#include <algorithm>
#include <cmath>
#include <QDebug>
#include <QPainter>

static double dotProduct(const point_t &vec1, const point_t &vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

static point_t vectorBetween(const point_t &from, const point_t &to)
{
    return { to.x - from.x, to.y - from.y };
}

static double crossProduct(const point_t &vec1, const point_t &vec2)
{
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

static bool isConvex(QVector<point_t> &polygon)
{
    if (polygon.size() < 3)
        return false;

    int sign = 0;
    for (size_t i = 0; i < polygon.size(); i++)
    {
        const point_t &p0 = polygon[i];
        const point_t &p1 = polygon[(i + 1) % polygon.size()];
        const point_t &p2 = polygon[(i + 2) % polygon.size()];

        point_t vec1 = vectorBetween(p0, p1);
        point_t vec2 = vectorBetween(p1, p2);

        double cross = crossProduct(vec1, vec2);

        if (std::abs(cross) < 1e-9)
            continue; // Игнорируем коллинеарные ребра

        int current_sign = (cross > 0) ? 1 : -1;

        if (sign == 0)
        {
            sign = current_sign;
        }
        else if (sign != current_sign)
        {
            return false;
        }
    }

    if (sign < 0)
    {
        std::reverse(polygon.begin(), polygon.end());
    }

    return true;
}

static point_t computeInnerNormal(const point_t &p1, const point_t &p2, const point_t &p3)
{
    point_t edge = vectorBetween(p1, p2);
    point_t normal;

    // Нормаль перпендикулярна ребру и направлена внутрь многоугольника
    normal = { -edge.y, edge.x };

    point_t testVec = vectorBetween(p2, p3);
    if (dotProduct(testVec, normal) < 0)
    {
        normal.x = -normal.x;
        normal.y = -normal.y;
    }

    double length = sqrt(normal.x * normal.x + normal.y * normal.y);
    if (length > 1e-9)
    {
        normal.x /= length;
        normal.y /= length;
    }

    return normal;
}

QDebug &operator<<(QDebug &os, const point_t &point)
{
    os << "{";

    os << point.x << ", " << point.y;

    os << "}";

    return os;
}


bool cut(QPainter &painter, const shape_t &clipper, const line_t &line, const QColor &resColor)
{
    QVector<point_t> polygon = clipper.vertices;
    if (!isConvex(polygon))
    {
        return false;
    }

    double tStart = 0.0;
    double tEnd = 1.0;

    const point_t &lineStart = line.start;
    const point_t &lineEnd = line.end;

    point_t lineDirection = vectorBetween(lineStart, lineEnd);

    for (int i = 0; i < polygon.size(); i++)
    {
        int next = (i + 1) % polygon.size();
        int nextNext = (i + 2) % polygon.size();

        const point_t &edgeStart = polygon[i];
        const point_t &edgeEnd = polygon[next];

        point_t normal = computeInnerNormal(edgeStart, edgeEnd, polygon[nextNext]);

        point_t w = vectorBetween(edgeStart, lineStart);

        double dN = dotProduct(lineDirection, normal);
        double wN = dotProduct(w, normal);

        if (std::abs(dN) <= 1e-9)
        {
            // Линия параллельна ребру
            if (wN < 0)
                return true; // Линия полностью вне
            else
                continue; // Линия внутри или на границе
        }

        double t = -wN / dN;

        if (dN > 0)
        {
            // Входная точка
            if (t <= tEnd)
                tStart = std::max(tStart, t);
            else
                return true; // Нет пересечения
        }
        else
        {
            // Выходная точка
            if (t >= tStart)
                tEnd = std::min(tEnd, t);
            else
                return true; // Нет пересечения
        }

        if (tStart > tEnd)
            return true; // Нет видимой части
    }

    if (tStart <= tEnd)
    {
        point_t clippedStart = { lineStart.x + lineDirection.x * tStart,
                                 lineStart.y + lineDirection.y * tStart };

        point_t clippedEnd = { lineStart.x + lineDirection.x * tEnd,
                               lineStart.y + lineDirection.y * tEnd };

        painter.setPen({QColor::fromRgb(39, 40, 41), 2});
        drawLine(painter, { clippedStart, clippedEnd, resColor });

        painter.setPen({resColor, 1});
        drawLine(painter, { clippedStart, clippedEnd, resColor });
    }

    return true;
}
