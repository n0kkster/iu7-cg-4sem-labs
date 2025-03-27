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

void createTimesGraph(double times1[][5], double times2[][5], int radiuses_count, 
                     double radiuses_start, double step, 
                     const std::string &datatype = "times")
{
    std::stringstream filename, dataname1, dataname2, exec_cmd;
    
    filename << "graphs/" << datatype << ".gp";
    dataname1 << "graphs/" << datatype << "_data1.dat";
    dataname2 << "graphs/" << datatype << "_data2.dat";
    exec_cmd << "gnuplot " << filename.str();

    std::ofstream gp(filename.str());

    std::ofstream data1(dataname1.str());
    for (int i = 0; i < radiuses_count; i++)
    {
        double radius = radiuses_start + i * step;
        data1 << radius << " "
              << times1[i][0] << " "
              << times1[i][1] << " "
              << times1[i][2] << " "
              << times1[i][3] << " "
              << times1[i][4] << "\n";
    }
    data1.close();

    std::ofstream data2(dataname2.str());
    for (int i = 0; i < radiuses_count; i++)
    {
        double radius = radiuses_start + i * step;
        data2 << radius << " "
              << times2[i][0] << " "
              << times2[i][1] << " "
              << times2[i][2] << " "
              << times2[i][3] << " "
              << times2[i][4] << "\n";
    }
    data2.close();

    gp << "set terminal pngcairo size 1200,800\n";
    gp << "set output 'graph.png'\n";
    gp << "set multiplot layout 2,1\n";

    gp << "set key horizontal above center\n";
    gp << "set key box\n";
    gp << "set key samplen 2 spacing 1.5\n";

    gp << "set title 'Окружность'\n";
    gp << "set xlabel 'Радиус (пикс.)'\n";
    gp << "set ylabel 'Время (мкс.)'\n";
    gp << "set grid\n";
    gp << "plot 'graphs/times_data1.dаt' using 1:2 with lines lt rgb 'red' notitle, \
             'graphs/times_data1.dаt' using 1:3 with lines lt rgb 'blue' notitle, \
             'graphs/times_data1.dаt' using 1:4 with lines lt rgb 'green' notitle, \
             'graphs/times_data1.dаt' using 1:5 with lines lt rgb 'orange' notitle, \
             'graphs/times_data1.dаt' using 1:6 with lines lt rgb 'purple' notitle, \
             NaN with points pt 5 ps 1.5 lt rgb 'red' title 'Каноническое уравнение', \
             NaN with points pt 5 ps 1.5 lt rgb 'blue' title 'Параметрическое уравнение', \
             NaN with points pt 5 ps 1.5 lt rgb 'green' title 'Брезенхем', \
             NaN with points pt 5 ps 1.5 lt rgb 'orange' title 'Алгоритм средней точки', \
             NaN with points pt 5 ps 1.5 lt rgb 'purple' title 'Встроенная функция'\n";

    gp << "set title 'Эллипс'\n";
    gp << "set xlabel 'Радиус (пикс.)'\n";
    gp << "set ylabel 'Время (мкс.)'\n";
    gp << "set grid\n";
    gp << "plot 'graphs/times_data2.dаt' using 1:2 with lines lt rgb 'red' notitle, \
             '' using 1:3 with lines lt rgb 'blue' notitle, \
             '' using 1:4 with lines lt rgb 'green' notitle, \
             '' using 1:5 with lines lt rgb 'orange' notitle, \
             '' using 1:6 with lines lt rgb 'purple' notitle\n";
    
    gp << "unset multiplot\n";
    gp.close();

    system(exec_cmd.str().c_str());
    system("open graph.png");
}

void MainWindow::onCompareTimeBtnClicked()
{
    QPainter painter(this); // FILLER

    constexpr int measures = 100;
    constexpr int radius_start = 1000, radius_step = 500, radius_stop = 10000;
    constexpr int radiuses_count = (radius_stop - radius_start) / radius_step + 1;
    constexpr int heatup = measures * 2;
    unsigned long start, end;

    size_t curr_radius = 0;
    double times_circle[radiuses_count][5] = {0};
    double times_ellipse[radiuses_count][5] = {0};

    void (*draw_funcs_circle[5])(QPainter &, const circle_t &, bool) = {drawCicrleCanonical, drawCicrleParametric, drawCircleBres, drawCicrleMidpoint, drawCircleBuiltin};
    void (*draw_funcs_ellipse[5])(QPainter &, const ellipse_t &, bool) = {drawEllipseCanonical, drawEllipseParametric, drawEllipseBres, drawEllipseMidpoint, drawEllipseBuiltin};

    circle_t test_circle = {.center = {0, 0}, .radius = 0, .method = BUILTIN, .color = QColor::fromRgba(0)};
    ellipse_t test_ellipse = {.center = {0, 0}, .rx = 0, .ry = 0, .method = BUILTIN, .color = QColor::fromRgba(0)};

    for (int radius = radius_start; radius <= radius_stop; radius += radius_step)
    {
        for (int method = 0; method < METHOD_COUNT; method++)
        {
            test_circle.method = (method_e)method;
            test_circle.radius = radius;

            test_ellipse.method = (method_e)method;
            test_ellipse.rx = radius / 2;
            test_ellipse.ry = radius * 2;

            for (int i = 0; i < measures + heatup; i++)
            {
                start = micros();
                draw_funcs_circle[method](painter, test_circle, true);
                end = micros();

                if (i >= heatup)
                    times_circle[curr_radius][method] += end - start;

                start = micros();
                draw_funcs_ellipse[method](painter, test_ellipse, true);
                end = micros();

                if (i >= heatup)
                    times_ellipse[curr_radius][method] += end - start;
            }

            times_circle[curr_radius][method] /= measures;
            times_ellipse[curr_radius][method] /= measures;
        }
        curr_radius++;
    }

    createTimesGraph(times_circle, times_ellipse, radiuses_count, radius_start, radius_step);
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
