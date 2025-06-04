#include "cut.h"
#include "line.h"

#include <QPainter>

const double EPSILON = 1e-9;

bool operator==(const point_t &a, const point_t &b)
{
    return std::abs(a.x - b.x) < EPSILON && std::abs(a.y - b.y) < EPSILON;
}

bool operator!=(const point_t &a, const point_t &b)
{
    return !(a == b);
}

bool operator<(const point_t &a, const point_t &b)
{
    if (std::abs(a.x - b.x) > EPSILON) // Если x различны
    {
        return a.x < b.x;
    }
    if (std::abs(a.y - b.y) > EPSILON) // Если x одинаковы, но y различны
    {
        return a.y < b.y;
    }
    return false; // Точки считаются равными (или очень близкими)
}

static point_t vectorBetween(const point_t &from, const point_t &to)
{
    return { to.x - from.x, to.y - from.y };
}

static double dotProduct(const point_t &vec1, const point_t &vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

// Функция для вычисления векторного произведения двух 2D-векторов (точнее, его Z-компоненты).
// Векторное произведение (cross product) для 2D векторов (x1, y1) и (x2, y2) равно x1*y2 - y1*x2.
// Его знак указывает на направление поворота от vec1 к vec2.
static double crossProduct(const point_t &vec1, const point_t &vec2)
{
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

// Функция для проверки, является ли полигон выпуклым.
// Также упорядочивает вершины полигона против часовой стрелки (CCW), если он выпуклый.
// Параметр 'polygon' может быть изменен (вершины могут быть переупорядочены).
static bool isConvex(QVector<point_t> &polygon)
{
    if (polygon.size() < 3) // Полигон должен иметь хотя бы 3 вершины.
        return false;

    int sign = 0; // Хранит знак векторного произведения для определения направления обхода.
    // Итерация по всем тройкам последовательных вершин (p0, p1, p2).
    for (size_t i = 0; i < polygon.size(); i++)
    {
        const point_t &p0 = polygon[i];
        const point_t &p1 = polygon[(i + 1) % polygon.size()]; // Следующая вершина с зацикливанием
        const point_t &p2 = polygon[(i + 2) % polygon.size()]; // Вершина после следующей

        point_t vec1 = vectorBetween(p0, p1); // Вектор ребра p0-p1
        point_t vec2 = vectorBetween(p1, p2); // Вектор ребра p1-p2
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
    {
        std::ranges::reverse(polygon);
    }
    return true; // Полигон выпуклый (и теперь CCW).
}

// Функция для вычисления внутренней нормали к ребру (p1, p2) полигона.
// p3 - следующая вершина после p2, используется для определения "внутренней" стороны.
// Предполагается, что полигон задан в порядке обхода против часовой стрелки (CCW).
static point_t computeInnerNormal(const point_t &p1, const point_t &p2, const point_t &p3)
{
    point_t edge = vectorBetween(p1, p2); // Вектор ребра p1p2
    // Перпендикулярный вектор к edge. Для CCW полигона, (-edge.y, edge.x) указывает налево от направления edge,
    // что и является направлением внутрь полигона.
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
    // Нормализация нормали (приведение к единичной длине) здесь не выполняется,
    // т.к. для алгоритма Сазерленда-Ходжмана важен только знак скалярного произведения,
    // а не величина нормали.
    return normal;
}

// Проверяет, лежит ли точка p на отрезке [seg_start, seg_end].
bool isPointOnSegment(const point_t &p, const point_t &seg_start, const point_t &seg_end)
{
    point_t vec_seg = vectorBetween(seg_start, seg_end); // Вектор самого отрезка
    point_t vec_start_p = vectorBetween(seg_start, p);   // Вектор от начала отрезка к точке p

    // 1. Проверка коллинеарности:
    // Если точка p, seg_start и seg_end коллинеарны, то векторное произведение (vec_seg, vec_start_p) должно быть равно 0.
    if (std::abs(crossProduct(vec_seg, vec_start_p)) > EPSILON)
        return false; // Точки не коллинеарны, значит p не на отрезке.

    // 2. Проверка принадлежности диапазону отрезка:
    // Если точки коллинеарны, нужно проверить, лежит ли p между seg_start и seg_end.
    // Проекция вектора vec_start_p на вектор vec_seg должна быть:
    // - не отрицательной (p не "левее" или "раньше" seg_start относительно направления vec_seg)
    // - не больше квадрата длины vec_seg (p не "правее" или "дальше" seg_end).
    double proj = dotProduct(vec_start_p, vec_seg);
    double seg_len_sq = dotProduct(vec_seg, vec_seg); // Квадрат длины отрезка

    // Проверяем, что проекция находится в диапазоне [0, длина_отрезка^2].
    // Используем EPSILON для учета погрешностей вычислений на концах отрезка.
    return proj >= -EPSILON && proj <= seg_len_sq + EPSILON;
}

// Разбивает данный отрезок (side_start, side_end) на меньшие отрезки (суб-сегменты)
// всеми точками из 'all_polygon_points', которые лежат на этом отрезке.
QVector<line_t> getSubSegments(const point_t &side_start, const point_t &side_end,
                                                    const QVector<point_t> &all_polygon_points)
{
    QVector<point_t> dots_on_segment; // Список точек, лежащих на отрезке, включая его концы.
    dots_on_segment.append(side_start);
    dots_on_segment.append(side_end);

    // Ищем все точки из 'all_polygon_points', которые лежат на текущем отрезке.
    for (const point_t &p : all_polygon_points)
    {
        // Не добавляем конечные точки отрезка снова, если они есть в all_polygon_points.
        if (p == side_start || p == side_end)
            continue;
        if (isPointOnSegment(p, side_start, side_end))
        {
            dots_on_segment.append(p);
        }
    }

    // Сортируем точки на отрезке. Используется operator< для point_t (лексикографическая сортировка).
    // Это работает корректно для точек, лежащих на одной прямой.
    std::sort(dots_on_segment.begin(), dots_on_segment.end());

    // Удаляем дубликаты точек, которые могли возникнуть из-за погрешностей или одинаковых точек во входных данных.
    dots_on_segment.erase(std::unique(dots_on_segment.begin(), dots_on_segment.end()), dots_on_segment.end());

    QVector<line_t> sections_list; // Список результирующих суб-сегментов.
    // Создаем суб-сегменты из отсортированных уникальных точек.
    for (int i = 0; i < dots_on_segment.size() - 1; ++i)
    {
        // Убеждаемся, что не создаем сегменты нулевой длины (на случай, если unique не идеально сработал).
        if (dots_on_segment[i] != dots_on_segment[i + 1])
        {
            sections_list.append({ dots_on_segment[i], dots_on_segment[i + 1] });
        }
    }
    return sections_list;
}

// Структура-компаратор для использования line_t в качестве ключа в std::map.
// Сравнивает пары лексикографически: сначала по start, затем по end.
struct PointPairLess
{
    bool operator()(const line_t &a, const line_t &b) const
    {
        if (a.start < b.start) // Сравниваем первые элементы пар
            return true;
        if (b.start < a.start) // Если a.start > b.start
            return false;
        // Если первые элементы равны, сравниваем вторые элементы пар
        return a.end < b.end;
    }
};

// Обрабатывает список отрезков (сторон), оставляя только те, которые встречаются нечетное количество раз.
// Перед подсчетом стороны нормализуются (меньшая точка ставится на первое место).
// Это используется, например, для определения "внешней" границы самопересекающегося полигона.
QVector<line_t> makeUniqueSides(QVector<line_t> &sides)
{
    // Нормализация каждого отрезка: меньшая точка (лексикографически) становится первой.
    // Это гарантирует, что отрезок (A, B) и (B, A) будут считаться одинаковыми.
    for (auto &side : sides)
    {
        if (side.end < side.start)
        {
            std::swap(side.start, side.end);
        }
    }

    // Подсчет количества вхождений каждого нормализованного отрезка.
    std::map<line_t, int, PointPairLess> counts;
    for (const auto &side : sides)
    {
        counts[side]++;
    }

    QVector<line_t> unique_sides; // Список отрезков, встречающихся нечетное число раз.
    // Отбираем отрезки с нечетным количеством вхождений.
    for (const auto &pair_entry : counts)
        if ((pair_entry.second % 2) == 1)
            unique_sides.append(pair_entry.first);

    return unique_sides;
}

// Находит точку пересечения отрезка (subj_p1, subj_p2) с бесконечной прямой,
// заданной точкой clip_p1 и вектором нормали normal_to_clip_edge.
// Эта прямая является одной из граней отсекателя.
point_t getIntersectionParametric(const point_t &subj_p1, const point_t &subj_p2, // Ребро отсекаемого полигона
                                  const point_t &clip_p1,                         // Точка на грани отсекателя
                                  const point_t &normal_to_clip_edge)             // Внутренняя нормаль к грани отсекателя
{
    point_t d_vec = vectorBetween(subj_p1, subj_p2); // Вектор направления ребра отсекаемого полигона D = P2 - P1
    point_t w_vec = vectorBetween(clip_p1, subj_p1); // Вектор от точки на грани отсекателя к началу ребра W = S1 - F (F - clip_p1, S1 - subj_p1)

    // Скалярное произведение вектора направления ребра на нормаль к грани отсекателя.
    // D_n = D • N
    double d_dot_normal = dotProduct(d_vec, normal_to_clip_edge);
    // Скалярное произведение вектора w_vec на нормаль.
    // W_n = W • N
    double w_dot_normal = dotProduct(w_vec, normal_to_clip_edge);

    // Если d_dot_normal близок к нулю, то ребро параллельно грани отсекателя (или коллинеарно ей).
    // В стандартном алгоритме Сазерленда-Ходжмана это обычно означает, что S и P находятся
    // по одну сторону от прямой отсечения (или на ней), и логика выбора точек это обработает.
    // Если бы пересечение все же требовалось, и линии коллинеарны, то это особый случай.
    // Здесь, для простоты, если они параллельны, можно вернуть одну из точек (например, subj_p1),
    // так как вызывающая логика (S внутри / P снаружи или наоборот) подразумевает, что они не параллельны
    // таким образом, чтобы вызвать деление на ноль, если пересечение действительно нужно.
    if (std::abs(d_dot_normal) < EPSILON)
    {
        // Это может произойти, если ребро полигона параллельно грани отсечения.
        // В контексте алгоритма, если S и P по разные стороны, d_dot_normal не должен быть 0.
        // Если они по одну сторону, пересечение не ищется.
        // Если они коллинеарны и на линии отсечения, то subj_p1 уже на линии.
        return subj_p1; // Возвращаем одну из точек как "безопасное" значение.
    }

    // Параметр t для параметрического уравнения прямой subj_p1 + t * d_vec = P(t)
    // t = - (W • N) / (D • N)
    double t = -w_dot_normal / d_dot_normal;

    // Координаты точки пересечения: I = subj_p1 + d_vec * t
    return { subj_p1.x + d_vec.x * t, subj_p1.y + d_vec.y * t };
}


// Отсекает полигон subject_polygon одной бесконечной прямой (гранью отсекателя).
// Грань задана точками clip_edge_p1, clip_edge_p2.
// inside_direction_ref_p - третья точка отсекателя, помогающая определить внутреннюю нормаль.
QVector<point_t> clipPolygonAgainstEdge(const QVector<point_t> &subject_polygon, const point_t &clip_edge_p1,
                                        const point_t &clip_edge_p2, const point_t &inside_direction_ref_p)
{
    QVector<point_t> result_polygon; // Результирующий полигон после отсечения этой гранью.
    if (subject_polygon.isEmpty())   // Если исходный полигон пуст, нечего отсекать.
        return result_polygon;

    // Вычисляем внутреннюю нормаль к грани отсекателя.
    // Предполагается, что отсекатель (cutter) имеет CCW обход.
    point_t normal = computeInnerNormal(clip_edge_p1, clip_edge_p2, inside_direction_ref_p);

    point_t S = subject_polygon.last(); // S - предыдущая обработанная вершина отсекаемого полигона.
    // Итерация по всем ребрам отсекаемого полигона (ребро S-P).
    for (int i = 0; i < subject_polygon.size(); ++i)
    {
        point_t P = subject_polygon[i]; // P - текущая вершина отсекаемого полигона.

        // Проверяем положение S и P относительно грани отсечения.
        // Вектор от точки на грани (clip_edge_p1) к S: vec_S = S - clip_edge_p1
        // Вектор от точки на грани (clip_edge_p1) к P: vec_P = P - clip_edge_p1
        // Скалярное произведение с нормалью:
        //   > 0  => точка внутри (или на границе, если 0, и нормаль направлена внутрь)
        //   < 0  => точка снаружи
        //   = 0  => точка на линии грани
        double s_dot_prod = dotProduct(vectorBetween(clip_edge_p1, S), normal);
        double p_dot_prod = dotProduct(vectorBetween(clip_edge_p1, P), normal);

        bool s_is_inside = (s_dot_prod >= -EPSILON); // S внутри или на границе
        bool p_is_inside = (p_dot_prod >= -EPSILON); // P внутри или на границе

        // 1. Если обе точки (S и P) внутри: добавляем P в результат.
        if (s_is_inside && p_is_inside)
        {
            result_polygon.append(P);
        }
        // 2. Если S внутри, а P снаружи: ребро S-P пересекает грань изнутри наружу.
        //    Добавляем точку пересечения I в результат.
        else if (s_is_inside && !p_is_inside)
        {
            result_polygon.append(getIntersectionParametric(S, P, clip_edge_p1, normal));
        }
        // 3. Если S снаружи, а P внутри: ребро S-P пересекает грань снаружи внутрь.
        //    Добавляем точку пересечения I, а затем точку P в результат.
        else if (!s_is_inside && p_is_inside)
        {
            result_polygon.append(getIntersectionParametric(S, P, clip_edge_p1, normal));
            result_polygon.append(P);
        }
        // 4. Если обе точки (S и P) снаружи: ничего не добавляем.

        S = P; // Переходим к следующему ребру: P становится новой S.
    }
    return result_polygon; // Возвращаем полигон, отсеченный текущей гранью.
}

// Основная функция отсечения полигона 'subject_shape' выпуклым полигоном 'cutter_shape'.
// Использует алгоритм Сазерленда-Ходжмана.
// 'painter' используется для отрисовки результата.
// 'resColor' - цвет для отрисовки отсеченного полигона.
bool cut(QPainter &painter, const shape_t &cutter_shape, const shape_t &subject_shape, const QColor &resColor)
{
    QVector<point_t> cutter_vertices = cutter_shape.vertices;   // Вершины отсекателя (клиппера)
    QVector<point_t> subject_vertices = subject_shape.vertices; // Вершины отсекаемого полигона

    // Проверка на минимальное количество вершин для полигона.
    if (subject_vertices.size() < 3)
        return false; // Отсекаемый объект не является полигоном.

    // Проверка, является ли отсекатель выпуклым.
    // isConvex также упорядочивает вершины cutter_vertices против часовой стрелки (CCW).
    if (!isConvex(cutter_vertices))
        return false; // Отсекатель должен быть выпуклым для алгоритма Сазерленда-Ходжмана.
    if (cutter_vertices.size() < 3)
        return false; // Отсекатель не является полигоном.


    QVector<point_t> clipped_polygon = subject_vertices; // Начинаем с исходного отсекаемого полигона.

    // Итерация по всем ребрам выпуклого отсекателя.
    // Каждое ребро отсекателя определяет одну отсекающую плоскость (линию в 2D).
    for (int i = 0; i < cutter_vertices.size(); ++i)
    {
        // Если на каком-то шаге полигон "исчез" (все точки отсеклись), прекращаем.
        if (clipped_polygon.isEmpty()) // Или clipped_polygon.size() < 1, но isEmpty() лучше
            break;

        // Текущее ребро отсекателя: (p1, p2)
        const point_t &p1 = cutter_vertices[i];
        const point_t &p2 = cutter_vertices[(i + 1) % cutter_vertices.size()]; // Следующая вершина с зацикливанием

        // Третья точка p3_ref используется для определения "внутренней" стороны ребра (p1, p2).
        // Так как отсекатель выпуклый и CCW, следующая вершина (p2, p3_ref) будет определять "внутренний" поворот.
        const point_t &p3_ref = cutter_vertices[(i + 2) % cutter_vertices.size()];

        // Отсекаем текущий `clipped_polygon` ребром (p1, p2) отсекателя.
        // Результат становится новым `clipped_polygon` для следующей итерации.
        clipped_polygon = clipPolygonAgainstEdge(clipped_polygon, p1, p2, p3_ref);
    }

    // Если после всех отсечений у полигона осталось менее 3 вершин, он выродился (в точку, отрезок или пуст).
    if (clipped_polygon.size() < 3)
        return false; // Результат не является полигоном.

    // Отрисовка результирующего отсеченного полигона.
    painter.setPen({resColor, 3}); // Устанавливаем цвет и толщину линии.

    for (size_t i = 0; i < clipped_polygon.size(); i++)
    {
        // Рисуем ребро от clipped_polygon[i] к clipped_polygon[(i + 1) % clipped_polygon.size()]
        drawLine(painter, {clipped_polygon[i], clipped_polygon[(i + 1) % clipped_polygon.size()], resColor}); // Передаем цвет в line_t
    }

    return true;
}