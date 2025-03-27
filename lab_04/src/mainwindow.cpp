#include <QMessageBox>
#include <QColorDialog>

#include <fstream>
#include <string>

#include <random>
#include <sys/time.h>

#include "mainwindow.h"

#include "bresenham.h"
#include "canonical.h"
#include "parametric.h"
#include "midpoint.h"
#include "builtin.h"

#include "out/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->drawCircleBtn, &QPushButton::clicked, this, &MainWindow::onDrawCircleBtnClicked);
    connect(ui->drawEllipseBtn, &QPushButton::clicked, this, &MainWindow::onDrawEllipseBtnClicked);

    connect(ui->drawSpectreCircleBtn, &QPushButton::clicked, this, &MainWindow::onDrawSpectreCircleBtnClicked);
    connect(ui->drawSpectreEllipseBtn, &QPushButton::clicked, this, &MainWindow::onDrawSpectreEllipseBtnClicked);

    connect(ui->lineColorBtn, &QPushButton::clicked, this, &MainWindow::onLineColorBtnClicked);
    connect(ui->bgColorBtn, &QPushButton::clicked, this, &MainWindow::onBgColorBtnClicked);
    connect(ui->clearScreenBtn, &QPushButton::clicked, this, &MainWindow::onClearScreenBtnClicked);

    connect(ui->compareTimeBtn, &QPushButton::clicked, this, &MainWindow::onCompareTimeBtnClicked);

    setLineColorDisplayColor("white");
}

void MainWindow::onDrawCircleBtnClicked()
{
    double xc, yc, radius;
    bool ok;
    method_e method;

    xc = ui->xcInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата X центра должна быть вещественным числом!");
        return;
    }

    yc = ui->ycInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата Y центра должна быть вещественным числом!");
        return;
    }

    radius = ui->circleRadiusInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Радиус должен быть вещественным числом!");
        return;
    }

    method = (method_e)ui->algoDropDown->currentIndex();
    ui->planeWidget->addCircle({{xc, yc}, radius, method, ui->lineColorDisplay->palette().color(QPalette::Window)});
    ui->planeWidget->viewport()->update();
}

void MainWindow::onDrawEllipseBtnClicked()
{
    double xc, yc, rx, ry;
    bool ok;
    method_e method;

    xc = ui->xcInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата X центра должна быть вещественным числом!");
        return;
    }

    yc = ui->ycInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата Y центра должна быть вещественным числом!");
        return;
    }

    rx = ui->ellipseRxInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Радиус по оси X должен быть вещественным числом!");
        return;
    }

    ry = ui->ellipseRyInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Радиус по оси Y должен быть вещественным числом!");
        return;
    }

    method = (method_e)ui->algoDropDown->currentIndex();
    ui->planeWidget->addEllipse({{xc, yc}, rx, ry, method, ui->lineColorDisplay->palette().color(QPalette::Window)});
    ui->planeWidget->viewport()->update();
}

void MainWindow::onDrawSpectreCircleBtnClicked()
{
    double step, xc, yc, radius;
    int count;

    bool ok;
    method_e method;

    xc = ui->xcSpecInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата X центра должна быть вещественным числом!");
        return;
    }

    yc = ui->ycSpecInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата Y центра должна быть вещественным числом!");
        return;
    }

    radius = ui->circleRadiusSpecInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Радиус должен быть вещественным числом!");
        return;
    }

    step = ui->stepRadiusInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Шаг радиуса должен быть вещественным числом!");
        return;
    }

    count = ui->shapesCountInp->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Количество фигур должно быть целым числом!");
        return;
    }

    method = (method_e)ui->algoDropDown->currentIndex();
    ui->planeWidget->addSpectreCircle({{xc, yc}, radius, method, ui->lineColorDisplay->palette().color(QPalette::Window)},
                                      step, count);
    ui->planeWidget->viewport()->update();
}

void MainWindow::onDrawSpectreEllipseBtnClicked()
{
    double step, xc, yc, rx, ry;
    int count;

    bool ok;
    method_e method;

    xc = ui->xcSpecInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата X центра должна быть вещественным числом!");
        return;
    }

    yc = ui->ycSpecInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата Y центра должна быть вещественным числом!");
        return;
    }

    rx = ui->ellipseRxSpecInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Радиус по оси X должен быть вещественным числом!");
        return;
    }

    ry = ui->ellipseRySpecInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Радиус по оси Y должен быть вещественным числом!");
        return;
    }

    step = ui->stepRadiusInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Шаг радиуса должен быть вещественным числом!");
        return;
    }

    count = ui->shapesCountInp->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Количество фигур должно быть целым числом!");
        return;
    }

    method = (method_e)ui->algoDropDown->currentIndex();
    ui->planeWidget->addSpectreEllipse({{xc, yc}, rx, ry, method, ui->lineColorDisplay->palette().color(QPalette::Window)},
                                       step, count);
    ui->planeWidget->viewport()->update();
}

void MainWindow::onLineColorBtnClicked()
{
    QColor lineColor = QColorDialog::getColor();
    setLineColorDisplayColor(lineColor);
}

void MainWindow::onBgColorBtnClicked()
{
    QColor bgColor = QColorDialog::getColor();
    ui->planeWidget->setBGColor(bgColor);
}

void MainWindow::onClearScreenBtnClicked()
{
    ui->planeWidget->clearPlane();
}

unsigned long long micros(void)
{
    struct timeval value;
    gettimeofday(&value, NULL);
    return (unsigned long long)value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}

void createTimesGraph(double times[][5], int radiuses_count, double radiuses_start, 
    double radiuses_stop, const std::string &datatype = "circle")
{

    std::stringstream filename, dataname, exec_cmd;

    filename << "graphs/" << datatype << ".gp";
    dataname << "graphs/" << datatype << "_data.dat";
    exec_cmd << "gnuplot " << filename.str();


    // Открываем файл для записи команд gnuplot
    std::ofstream gp(filename.str());

    // Вычисляем шаг между радиусами
    double step = (radiuses_stop - radiuses_start) / radiuses_count;

    // Создаем временный файл с данными
    std::ofstream data(dataname.str());
    for (int i = 0; i < radiuses_count; i++)
    {
        double radius = radiuses_start + i * step;
        data << radius << " "
             << times[i][0] << " "
             << times[i][1] << " "
             << times[i][2] << " "
             << times[i][3] << " "
             << times[i][4] << "\n";
    }
    data.close();

    // Записываем команды для gnuplot
    gp << "set terminal wxt 1 title 'Зaмер времени просчета'\n";
    gp << "set output 'graph.png'\n";
    gp << "set title 'Зависимость времени от радиуса'\n";
    gp << "set xlabel 'Радиус'\n";
    gp << "set ylabel 'Время'\n";
    gp << "set grid\n";
    gp << "plot '" << dataname.str() << "' using 1:2 with lines title 'Каноническое уравнение', \
             '' using 1:3 with lines title 'Параметрическое уравнение', \
             '' using 1:4 with lines title 'Брезенхем', \
             '' using 1:5 with lines title 'Алгоритм средней точки', \
             '' using 1:6 with lines title 'Встроенная функция'\n";
    gp << "pause mouse close\n";
    gp.close();

    // Выполняем gnuplot
    system(exec_cmd.str().c_str());
}

void MainWindow::onCompareTimeBtnClicked()
{
    QPainter painter; // FILLER

    constexpr int measures = 100;
    constexpr int radius_start = 1000, radius_step = 100, radius_stop = 10000;
    constexpr int radiuses_count = (radius_stop - radius_start) / radius_step;
    unsigned long start, end;

    size_t curr_radius = 0;
    double times[radiuses_count][5] = {0};

    std::vector<int> temp;

    void (*draw_funcs[5])(QPainter &, const circle_t &, bool) = {drawCicrleCanonical, drawCicrleParametric, drawCircleBres, drawCicrleMidpoint, drawCircleBuiltin};

    circle_t test_circle = {.center = {0, 0}, .radius = 0, .method = BUILTIN, .color = QColor::fromRgb(0, 0, 0)};

    for (int radius = radius_start; radius < radius_stop; radius += radius_step)
    {
        for (int method = 0; method < METHOD_COUNT; method++)
        {
            test_circle.method = (method_e)method;
            test_circle.radius = radius;

            for (int i = 0; i < measures; i++)
            {
                start = micros();
                draw_funcs[method](painter, test_circle, true);
                end = micros();
                // times[curr_radius][method] += end - start;
                temp.push_back(end - start);
            }

            std::ranges::sort(temp);

            if ((method_e)method == PARAMETRIC)
                qDebug() << temp;

            times[curr_radius][method] = temp[temp.size() / 2];

            temp.clear();
        }
        curr_radius++;
    }

    curr_radius = 0;

    // for (int radius = radius_start; radius < radius_stop; radius += radius_step)
    // {
    //     for (int method = 0; method < METHOD_COUNT; method++)
    //         qDebug() << curr_radius << method << times[curr_radius][method];
    //     curr_radius++;
    // }

    createTimesGraph(times, radiuses_count, radius_start, radius_stop);
}

void MainWindow::setLineColorDisplayColor(const QColor &color)
{
    QPalette palette = ui->lineColorDisplay->palette();
    palette.setColor(QPalette::Window, color);
    ui->lineColorDisplay->setAutoFillBackground(true);
    ui->lineColorDisplay->setPalette(palette);
    ui->lineColorDisplay->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
