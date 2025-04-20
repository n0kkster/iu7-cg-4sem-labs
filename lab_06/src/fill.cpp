#include "fill.h"

#include "common.h"

#include <QImage>
#include <QStack>

static bool check_pixel(const point_t &pixel, const QImage &buffer, const QColor &border_color,
                        const QColor &fill_color)
{
    if (!(pixel.x >= 0 && pixel.x < buffer.width() && pixel.y >= 0 && pixel.y < buffer.height()))
        return false;

    QColor curr_color = buffer.pixelColor(pixel.x, pixel.y);
    
    return curr_color != fill_color && curr_color != border_color;
}

void fill2(QPainter &painter, const QImage &buffer, const point_t seed, const QColor &border_color,
           const QColor &fill_color)
{
    QStack<point_t> points;
    point_t curr;

    points.push(seed);

    while (!points.empty())
    {
        curr = points.pop();
        painter.drawPoint(curr.x, curr.y);

        if (check_pixel({ curr.x + 1, curr.y }, buffer, border_color, fill_color))
            points.push({ curr.x + 1, curr.y });

        if (check_pixel({ curr.x, curr.y + 1 }, buffer, border_color, fill_color))
            points.push({ curr.x, curr.y + 1 });

        if (check_pixel({ curr.x - 1, curr.y }, buffer, border_color, fill_color))
            points.push({ curr.x - 1, curr.y });

        if (check_pixel({ curr.x, curr.y - 1 }, buffer, border_color, fill_color))
            points.push({ curr.x, curr.y - 1 });
    }
}
