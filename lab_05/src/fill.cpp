#include "fill.h"

#include "common.h"

#include <QDebug>

static bool checkEdge(int y, const edge_t &edge)
{
    int start = edge.start.y, end = edge.end.y;

    if (start < end)
        return start < y && y < end;
    else if (start > end)
        return end < y && y < start;
    else
        return true;
}

static int count(const std::vector<point_t> &points, int x)
{
    int cnt = 0;
    for (const auto &p : points)
        if (p.x == x)
            cnt++;
    return cnt;
}

static int getIntersectionNearest(int y, const edge_t &edge)
{
    int x1 = edge.start.x, y1 = edge.start.y, x2 = edge.end.x, y2 = edge.end.y;
    double intersection = ((x1 * y2 - y1 * x2) - (x1 - x2) * y) / (double)(y2 - y1);
    return lrintf64(intersection + 0.5);
}

void updateDimensions(dimensions_t &dim, const shape_t &shape)
{
    for (const auto &v : shape.vertices)
    {
        dim.xmin = std::min(dim.xmin, v.x);
        dim.xmax = std::max(dim.xmax, v.x);
        dim.ymin = std::min(dim.ymin, v.y);
        dim.ymax = std::max(dim.ymax, v.y);
    }
}

void outline(QPainter &painter, std::map<int, std::vector<point_t>> &outline_points, const shape_t &shape,
             const dimensions_t &dim)
{
    int x;

    for (int y = dim.ymin; y < dim.ymax; y++)
    {
        for (const auto &edge : shape.edges)
        {
            if (!checkEdge(y, edge))
                continue;
            x = getIntersectionNearest(y, edge);
            painter.drawPoint(x, y);
            outline_points[y].push_back({ x, y });
        }
    }

    for (const auto &p : shape.vertices)
    {
        auto it = outline_points.find(p.y);
        if (it == outline_points.end())
        {
            outline_points[p.y].push_back(p);
            outline_points[p.y].push_back(p);
        }
        else
            if (it->second.size() % 2 == 1)
                it->second.push_back(p);
    }
}

void fill(QPainter &painter, const std::map<int, std::vector<point_t>> &outline_points, const QColor &color,
          const dimensions_t &dim)
{
    painter.setPen({ color, 1 });

    bool inside;
    for (int y = dim.ymin; y < dim.ymax; y++)
    {
        inside = false;
        for (int x = dim.xmin; x < dim.xmax; x++)
        {
            if (count(outline_points.at(y), x) % 2 != 0)
                inside = !inside;
            if (inside)
                painter.drawPoint(x, y);
        }
    }

    painter.setPen({ Qt::white, 1 });
}
