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

// Функция для вычисления векторного произведения двух 2D-векторов.
// Векторное произведение (cross product) для 2D векторов (x1, y1) и (x2, y2) равно x1*y2 - y1*x2.
// Его знак указывает на направление поворота от vec1 к vec2.
static double crossProduct(const point_t &vec1, const point_t &vec2)
{
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

// Функция для проверки, является ли полигон выпуклым.
// Также упорядочивает вершины полигона против часовой стрелки (CCW), если он выпуклый.
// Параметр 'polygon' может быть изменен (вершины могут быть переупорядочены).
static bool isConvex(QVector<point_t> &poly)
{
    if (poly.size() < 3) // Полигон должен иметь хотя бы 3 вершины.
        return false;

    int sign = 0; // Хранит знак векторного произведения для определения направления обхода.
    // Итерация по всем тройкам последовательных вершин (p0, p1, p2).
    for (size_t i = 0; i < poly.size(); i++)
    {
        const point_t &p0 = poly[i];
        const point_t &p1 = poly[(i + 1) % poly.size()]; // Следующая вершина с зацикливанием
        const point_t &p2 = poly[(i + 2) % poly.size()]; // Вершина после следующей

        point_t vec1 = vectorBetween(p0, p1);    // Вектор ребра p0-p1
        point_t vec2 = vectorBetween(p1, p2);    // Вектор ребра p1-p2
        double cross = crossProduct(vec1, vec2); // Векторное произведение

        // Если точки коллинеарны (лежат на одной прямой), cross будет близок к 0.
        // Пропускаем такие случаи, так как они не нарушают выпуклость, но и не определяют знак.
        if (std::abs(cross) < EPSILON)
            continue;

        int current_sign = (cross > 0) ? 1 : -1; // Определяем знак текущего поворота

        if (sign == 0) // Если это первый ненулевой поворот, запоминаем его знак.
            sign = current_sign;

        // Если знак текущего поворота отличается от предыдущих, полигон невыпуклый.
        else if (sign != current_sign)
            return false;
    }

    // Если все повороты были в одну сторону (или коллинеарны), полигон выпуклый.
    // Алгоритм Сазерленда-Ходжмана ожидает, что отсекатель будет иметь обход против часовой стрелки (CCW).
    // Если sign < 0, это означает обход по часовой стрелке (CW) для системы координат, где Y растет вверх.
    // В этом случае инвертируем порядок вершин, чтобы обеспечить CCW.
    if (sign < 0)
        std::ranges::reverse(poly);
    return true; // Полигон выпуклый (и теперь CCW).
}

// Функция для вычисления внутренней нормали к ребру (p1, p2) полигона.
// p3 - следующая вершина после p2, используется для определения "внутренней" стороны.
// Предполагается, что полигон задан в порядке обхода против часовой стрелки (CCW).
static point_t computeInnerNormal(const point_t &p1, const point_t &p2, const point_t &p3)
{
    point_t edge = vectorBetween(p1, p2); // Вектор ребра p1p2
    // Перпендикулярный вектор к edge. Для CCW полигона, (-edge.y, edge.x) указывает налево от направления
    // edge, что и является направлением внутрь полигона.
    point_t normal = { -edge.y, edge.x };

    // Проверка, действительно ли нормаль направлена внутрь.
    // Создаем вектор от p2 (конец ребра) к p3 (следующая точка полигона).
    point_t testVec = vectorBetween(p2, p3);
    // Если скалярное произведение testVec и normal отрицательно,
    // это значит, что угол между ними > 90 градусов, т.е. normal направлена наружу.
    // В этом случае инвертируем нормаль.
    // Это может произойти, если исходное предположение о направлении (-edge.y, edge.x) было неверным
    // из-за особенностей системы координат или если полигон был CW (хотя isConvex должен это исправить).
    if (dotProduct(testVec, normal) < 0)
    {
        normal.x = -normal.x;
        normal.y = -normal.y;
    }
    return normal;
}

// Находит точку пересечения отрезка (poly_p1, poly_p2) с бесконечной прямой,
// заданной точкой cut_p1 и вектором нормали normal_to_cut_edge.
// Эта прямая является одной из граней отсекателя.
point_t getIntersectionParametric(const point_t &poly_p1,
                                  const point_t &poly_p2,             // Ребро отсекаемого полигона
                                  const point_t &cut_p1,             // Точка на грани отсекателя
                                  const point_t &normal_to_cut_edge) // Внутренняя нормаль к грани отсекателя
{
    point_t d_vec =
        vectorBetween(poly_p1, poly_p2); // Вектор направления ребра отсекаемого полигона D = P2 - P1
    point_t w_vec = vectorBetween(
        cut_p1,
        poly_p1); // Вектор от точки на грани отсекателя к началу ребра W = S1 - F (F - cut_p1, S1 - poly_p1)

    // Скалярное произведение вектора направления ребра на нормаль к грани отсекателя.
    // D_n = DN
    double d_dot_normal = dotProduct(d_vec, normal_to_cut_edge);
    // Скалярное произведение вектора w_vec на нормаль.
    // W_n = WN
    double w_dot_normal = dotProduct(w_vec, normal_to_cut_edge);

    // Параметр t для параметрического уравнения прямой poly_p1 + t * d_vec = P(t)
    // t = - (W • N) / (D • N)
    double t = -w_dot_normal / d_dot_normal;

    // Координаты точки пересечения: I = poly_p1 + d_vec * t
    return { poly_p1.x + d_vec.x * t, poly_p1.y + d_vec.y * t };
}

// Вычисляем видимость точки по алгоритму Сазерленда Ходжмена.
// Берем векторное между вектором от точки ребра к искомой и вектором ребра. Если >= 0, то видима.
bool isVisible(const point_t &p, const point_t &e1, const point_t &e2)
{
    const point_t pv = vectorBetween(p, e1);
    const point_t sv = vectorBetween(e1, e2);
    double res = crossProduct(pv, sv);

    return res >= 0;
}

// Отсекает полигон poly одной бесконечной прямой (гранью отсекателя).
// Грань задана точками cut_edge_p1, cut_edge_p2.
// inside_direction_p - третья точка отсекателя, помогающая определить внутреннюю нормаль.
QVector<point_t> cutPolygonAgainstEdge(const QVector<point_t> &poly, const point_t &cut_edge_p1,
                                        const point_t &cut_edge_p2, const point_t &inside_direction_p)
{
    QVector<point_t> result_polygon; // Результирующий полигон после отсечения этой гранью.
    if (poly.isEmpty())             // Если исходный полигон пуст, нечего отсекать.
        return result_polygon;

    // Вычисляем внутреннюю нормаль к грани отсекателя.
    // Предполагается, что отсекатель (cutter) имеет CCW обход.
    point_t normal = computeInnerNormal(cut_edge_p1, cut_edge_p2, inside_direction_p);

    point_t S = poly.last(); // S - предыдущая обработанная вершина отсекаемого полигона.
    // Итерация по всем ребрам отсекаемого полигона (ребро S-P).
    for (int i = 0; i < poly.size(); ++i)
    {
        point_t P = poly[i]; // P - текущая вершина отсекаемого полигона.

        bool s_is_inside = isVisible(S, cut_edge_p1, cut_edge_p2);
        bool p_is_inside = isVisible(P, cut_edge_p1, cut_edge_p2);

        // 1. Если обе точки (S и P) внутри: добавляем P в результат.
        if (s_is_inside && p_is_inside)
            result_polygon.append(P);
        // 2. Если S внутри, а P снаружи: ребро S-P пересекает грань изнутри наружу.
        //    Добавляем точку пересечения I в результат.
        else if (s_is_inside && !p_is_inside)
            result_polygon.append(getIntersectionParametric(S, P, cut_edge_p1, normal));
        // 3. Если S снаружи, а P внутри: ребро S-P пересекает грань снаружи внутрь.
        //    Добавляем точку пересечения I, а затем точку P в результат.
        else if (!s_is_inside && p_is_inside)
        {
            result_polygon.append(getIntersectionParametric(S, P, cut_edge_p1, normal));
            result_polygon.append(P);
        }
        // 4. Если обе точки (S и P) снаружи: ничего не добавляем.

        S = P; // Переходим к следующему ребру: P становится новой S.
    }
    return result_polygon; // Возвращаем полигон, отсеченный текущей гранью.
}

// Основная функция отсечения полигона poly выпуклым полигоном cutter.
// Использует алгоритм Сазерленда-Ходжмана.
// 'painter' используется для отрисовки результата.
// 'resColor' - цвет для отрисовки отсеченного полигона.
bool cut(QPainter &painter, const shape_t &cutter, const shape_t &poly, const QColor &resColor)
{
    QVector<point_t> cutter_vertices = cutter.vertices; // Вершины отсекателя (клиппера)
    QVector<point_t> poly_vertices = poly.vertices;   // Вершины отсекаемого полигона

    // Проверка на минимальное количество вершин для полигона.
    if (poly_vertices.size() < 3)
        return false; // Отсекаемый объект не является полигоном.

    if (cutter_vertices.size() < 3)
        return false; // Отсекатель не является полигоном.

    // Проверка, является ли отсекатель выпуклым.
    // isConvex также упорядочивает вершины cutter_vertices против часовой стрелки (CCW).
    if (!isConvex(cutter_vertices))
        return false; // Отсекатель должен быть выпуклым для алгоритма Сазерленда-Ходжмана.

    QVector<point_t> cutped_polygon = poly_vertices; // Начинаем с исходного отсекаемого полигона.

    // Итерация по всем ребрам выпуклого отсекателя.
    // Каждое ребро отсекателя определяет одну отсекающую плоскость (линию в 2D).
    for (int i = 0; i < cutter_vertices.size(); ++i)
    {
        // Если на каком-то шаге полигон "исчез" (все точки отсеклись), прекращаем.
        if (cutped_polygon.isEmpty()) // Или cutped_polygon.size() < 1, но isEmpty() лучше
            break;

        // Текущее ребро отсекателя: (p1, p2)
        const point_t &p1 = cutter_vertices[i];
        const point_t &p2 =
            cutter_vertices[(i + 1) % cutter_vertices.size()]; // Следующая вершина с зацикливанием

        // Третья точка p3_ref используется для определения "внутренней" стороны ребра (p1, p2).
        // Так как отсекатель выпуклый и CCW, следующая вершина (p2, p3_ref) будет определять "внутренний"
        // поворот.
        const point_t &p3 = cutter_vertices[(i + 2) % cutter_vertices.size()];

        // Отсекаем текущий `cutped_polygon` ребром (p1, p2) отсекателя.
        // Результат становится новым `cutped_polygon` для следующей итерации.
        cutped_polygon = cutPolygonAgainstEdge(cutped_polygon, p1, p2, p3);
    }

    // Если после всех отсечений у полигона осталось менее 3 вершин, он выродился (в точку, отрезок или пуст).
    if (cutped_polygon.size() < 3)
        return false; // Результат не является полигоном.

    // Отрисовка результирующего отсеченного полигона.
    painter.setPen({ resColor, 2 }); // Устанавливаем цвет и толщину линии.

    for (size_t i = 0; i < cutped_polygon.size(); i++)
    {
        // Рисуем ребро от cutped_polygon[i] к cutped_polygon[(i + 1) % cutped_polygon.size()]
        drawLine(painter, { cutped_polygon[i], cutped_polygon[(i + 1) % cutped_polygon.size()],
                            resColor }); // Передаем цвет в line_t
    }

    return true;
}
