#include "cut.h"

#include "line.h"

#include <QPainter>
#include <QRect>

static std::array<int, 4> getPointBitCode(const std::array<int, 4> &window, const point_t &point)
{
    std::array<int, 4> res = { 0, 0, 0, 0 };
    int xl, yt, xr, yb;

    xl = window[0], xr = window[1], yb = window[2], yt = window[3];

    if (point.x < xl)
        res[0] = 1;

    if (point.x > xr)
        res[1] = 1;

    if (point.y > yb)
        res[2] = 1;

    if (point.y < yt)
        res[3] = 1;

    return res;
}

static int sum(const std::array<int, 4> T) { return T[0] + T[1] + T[2] + T[3]; }

static int logical_mul(const std::array<int, 4> T1, const std::array<int, 4> T2)
{
    int mul = 0;
    for (int i = 0; i < 4; i++)
        mul |= T1[i] * T2[i];

    return mul;
}

static char calcVisibility(const std::array<int, 4> T1, const std::array<int, 4> T2)
{
    int s1 = sum(T1);
    int s2 = sum(T2);
    int prod = 0;
    char pr = 0;

    if (s1 == 0 && s2 == 0)
        pr = 1;
    else
    {
        prod = logical_mul(T1, T2);
        if (prod == 1)
            pr = -1;
        else
            pr = 0;
    }

    return pr;
}

void cut(QPainter &painter, const QRect &rect, line_t line, QColor resColor)
{
    char fl = 0;
    double m;

    std::array<int, 4> window;
    rect.getCoords(&window[0], &window[3], &window[1], &window[2]);

    if (line.start.x == line.end.x)
        fl = -1;
    else
        m = (line.end.y - line.start.y) / (double)(line.end.x - line.start.x);

    if (m == 0)
        fl = 1;

    for (int i = 0; i < window.size(); i++)
    {
        auto T1 = getPointBitCode(window, line.start);
        auto T2 = getPointBitCode(window, line.end);

        int pr = calcVisibility(T1, T2);

        if (pr == -1)
            return;

        if (pr == 1)
        {
            painter.setPen({ resColor, 1 });
            drawLine(painter, line);
            return;
        }

        if (T1[i] == T2[i])
            continue;

        if (T1[i] == 0)
            std::swap<point_t>(line.start, line.end);

        if (fl == -1)
            line.start.y = window[i];
        else
        {
            if (i < 2)
            {
                line.start.y += lrintf64(m * (window[i] - line.start.x));
                line.start.x = window[i];
            }
            else
            {
                line.start.x += lrintf64((window[i] - line.start.y) / m);
                line.start.y = window[i];
            }
        }
    }

    painter.setPen({ resColor, 1 });
    drawLine(painter, line);
}
