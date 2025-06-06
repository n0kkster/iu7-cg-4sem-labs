#include "cut.h"

#include "common.h"
#include "line.h"

#include <QPainter>

const double EPSILON = 1e-9;

static point_t vectorBetween(const point_t &from, const point_t &to)
{
    return { to.x - from.x, to.y - from.y };
}

static double dotProduct(const point_t &vec1, const point_t &vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

static double crossProduct(const point_t &vec1, const point_t &vec2)
{
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

static bool isConvex(QVector<point_t> &poly)
{
    if (poly.size() < 3)
        return false;

    int sign = 0;

    for (size_t i = 0; i < poly.size(); i++)
    {
        const point_t &p0 = poly[i];
        const point_t &p1 = poly[(i + 1) % poly.size()];
        const point_t &p2 = poly[(i + 2) % poly.size()];

        point_t vec1 = vectorBetween(p0, p1);
        point_t vec2 = vectorBetween(p1, p2);
        double cross = crossProduct(vec1, vec2);

        if (std::abs(cross) < EPSILON)
            continue;

        int current_sign = (cross > 0) ? 1 : -1;

        if (sign == 0)
            sign = current_sign;

        else if (sign != current_sign)
            return false;
    }

    if (sign < 0)
        std::ranges::reverse(poly);
    return true;
}

static point_t computeInnerNormal(const point_t &p1, const point_t &p2, const point_t &p3)
{
    point_t edge = vectorBetween(p1, p2);

    point_t normal = { -edge.y, edge.x };

    point_t testVec = vectorBetween(p2, p3);

    if (dotProduct(testVec, normal) < 0)
    {
        normal.x = -normal.x;
        normal.y = -normal.y;
    }
    return normal;
}

point_t getIntersectionParametric(const point_t &poly_p1, const point_t &poly_p2, const point_t &cut_p1,
                                  const point_t &normal_to_cut_edge)
{
    point_t d_vec = vectorBetween(poly_p1, poly_p2);
    point_t w_vec = vectorBetween(cut_p1, poly_p1);

    double d_dot_normal = dotProduct(d_vec, normal_to_cut_edge);

    double w_dot_normal = dotProduct(w_vec, normal_to_cut_edge);

    double t = -w_dot_normal / d_dot_normal;

    return { poly_p1.x + d_vec.x * t, poly_p1.y + d_vec.y * t };
}

bool isVisible(const point_t &p, const point_t &e1, const point_t &e2)
{
    const point_t pv = vectorBetween(p, e1);
    const point_t sv = vectorBetween(e1, e2);
    double res = crossProduct(pv, sv);

    return res >= 0;
}

QVector<point_t> cutPolygonAgainstEdge(const QVector<point_t> &poly, const point_t &cut_edge_p1,
                                       const point_t &cut_edge_p2, const point_t &inside_direction_p)
{
    QVector<point_t> result_polygon;
    if (poly.isEmpty())
        return result_polygon;

    point_t normal = computeInnerNormal(cut_edge_p1, cut_edge_p2, inside_direction_p);

    point_t S = poly.last();

    for (int i = 0; i < poly.size(); ++i)
    {
        point_t P = poly[i];

        bool s_is_inside = isVisible(S, cut_edge_p1, cut_edge_p2);
        bool p_is_inside = isVisible(P, cut_edge_p1, cut_edge_p2);

        if (s_is_inside && p_is_inside)
            result_polygon.append(P);

        else if (s_is_inside && !p_is_inside)
            result_polygon.append(getIntersectionParametric(S, P, cut_edge_p1, normal));

        else if (!s_is_inside && p_is_inside)
        {
            result_polygon.append(getIntersectionParametric(S, P, cut_edge_p1, normal));
            result_polygon.append(P);
        }

        S = P;
    }
    return result_polygon;
}

bool cut(QPainter &painter, const shape_t &cutter, const shape_t &poly, const QColor &resColor)
{
    QVector<point_t> cutter_vertices = cutter.vertices;
    QVector<point_t> poly_vertices = poly.vertices;

    if (poly_vertices.size() < 3)
        return false;

    if (cutter_vertices.size() < 3)
        return false;

    if (!isConvex(cutter_vertices))
        return false;

    QVector<point_t> cutted_polygon = poly_vertices;

    for (int i = 0; i < cutter_vertices.size(); ++i)
    {

        if (cutted_polygon.isEmpty())
            break;

        const point_t &p1 = cutter_vertices[i];
        const point_t &p2 = cutter_vertices[(i + 1) % cutter_vertices.size()];

        const point_t &p3 = cutter_vertices[(i + 2) % cutter_vertices.size()];

        cutted_polygon = cutPolygonAgainstEdge(cutted_polygon, p1, p2, p3);
    }

    if (cutted_polygon.size() < 3)
        return false;

    painter.setPen({ resColor, 2 });

    for (size_t i = 0; i < cutted_polygon.size(); i++)
    {

        drawLine(painter, { cutted_polygon[i], cutted_polygon[(i + 1) % cutted_polygon.size()], resColor });
    }

    return true;
}
