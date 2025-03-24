#include <QMessageBox>
#include <QColorDialog>

#include <fstream>
#include <iostream>

#include <random>
#include <sys/time.h>

#include "mainwindow.h"

#include "bresenham.h"

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

void createTimeHist(double a, double b, double c, double d, double e, const std::string& filename = "histogram.gp") 
{
    std::ofstream gpFile("graphs/" + filename);
        
    gpFile << "# Gnuplot script for histogram\n\n";
    gpFile << "set xlabel 'Алгоритмы'\n";
    gpFile << "set ylabel 'Время'\n";
    gpFile << "set grid\n";
    gpFile << "set style data histograms\n";
    gpFile << "set style histogram clustered gap 1\n";
    gpFile << "set style fill solid 1.0\n";
    gpFile << "set boxwidth 0.9\n";
    
    gpFile << "set yrange [0:1]\n";
    
    gpFile << "set terminal wxt 1 title 'Звмер времени работы алгоритмов'\n";
    
    gpFile << "plot '-' using 2:xtic(1) title 'мкс.'\n";
    gpFile << "'ЦДА' " << a << "\n";
    gpFile << "'Брез.(real)' " << b << "\n";
    gpFile << "'Брез.(int)' " << c << "\n";
    gpFile << "'Брез.(smooth)' " << d << "\n";
    gpFile << "'Ву' " << e << "\n";
    gpFile << "e\n";
    
    gpFile << "pause mouse close\n";
    
    gpFile.close();
}

void MainWindow::onCompareTimeBtnClicked()
{
    QPainter painter; // FILLER

    const int measures = 1000;
    unsigned long start, end;

    // createTimeHist(avg_time_dda, avg_time_bres_real, avg_time_bres_int, avg_time_bres_smooth, avg_time_wu, "time.gp");
    system("gnuplot graphs/time.gp");
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
