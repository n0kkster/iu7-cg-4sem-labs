#include <QMessageBox>
#include <QColorDialog>

#include <fstream>
#include <iostream>

#include <random>
#include <sys/time.h>

#include "mainwindow.h"

#include "dda.h"
#include "bresenham.h"
#include "wu.h"

#include "out/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->drawLineBtn, &QPushButton::clicked, this, &MainWindow::onDrawLineBtnClicked);
    connect(ui->drawSpectreBtn, &QPushButton::clicked, this, &MainWindow::onDrawSpectreBtnClicked);

    connect(ui->lineColorBtn, &QPushButton::clicked, this, &MainWindow::onLineColorBtnClicked);
    connect(ui->bgColorBtn, &QPushButton::clicked, this, &MainWindow::onBgColorBtnClicked);
    connect(ui->clearScreenBtn, &QPushButton::clicked, this, &MainWindow::onClearScreenBtnClicked);

    connect(ui->compareTimeBtn, &QPushButton::clicked, this, &MainWindow::onCompareTimeBtnClicked);
    connect(ui->compareStepsBtn, &QPushButton::clicked, this, &MainWindow::onCompareStepsBtnClicked);

    setLineColorDisplayColor("white");
}

void MainWindow::onDrawLineBtnClicked()
{
    double xs, ys, xe, ye;
    bool ok;
    method_e method;

    xs = ui->xsInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Начальная координата по оси X должна быть вещественным числом!");
        return;
    }

    ys = ui->ysInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Начальная координата по оси Y должна быть вещественным числом!");
        return;
    }

    xe = ui->xeInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Конечная координата по оси X должна быть вещественным числом!");
        return;
    }

    ye = ui->yeInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Конечная координата по оси Y должна быть вещественным числом!");
        return;
    }

    method = (method_e)ui->algoDropDown->currentIndex();
    ui->planeWidget->addLine({{xs, ys}, {xe, ye}, method, ui->lineColorDisplay->palette().color(QPalette::Window)});
    ui->planeWidget->viewport()->update();
}

void MainWindow::onDrawSpectreBtnClicked()
{
    double length, angle;
    bool ok;
    method_e method;

    length = ui->lengthInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Длина должна быть вещественным числом!");
        return;
    }

    angle = ui->angleInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Угол должен быть вещественным числом!");
        return;
    }

    method = (method_e)ui->algoDropDown->currentIndex();
    ui->planeWidget->addSpectre(length, method, angle, ui->lineColorDisplay->palette().color(QPalette::Window));
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

void MainWindow::genRandomLine(line_t &line, const int length)
{
    const int xs = 250, ys = 250;
    double lb = 0, ub = 360, angle;

    std::uniform_real_distribution<double> unif(lb, ub);
    std::default_random_engine re;

    angle = unif(re);

    line.color = {0, 0, 0}; // FILLER
    line.method = BUILTIN; // FILLER

    line.start = {xs, ys};
    line.end = {xs + (double)length, ys};

    ui->planeWidget->rotatePoint(angle, line.end, line.start);
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

    const int length = 250;
    const int measures = 1000;

    double avg_time_dda = 0, avg_time_bres_real = 0, avg_time_bres_int = 0, avg_time_bres_smooth = 0, avg_time_wu = 0;

    unsigned long start, end;

    line_t line;

    for (int i = 0; i < measures; i++)
    {
        genRandomLine(line, length);

        start = micros();
        dda(painter, line, true);
        end = micros();
        avg_time_dda += end - start;

        start = micros();
        bres_real(painter, line, true);
        end = micros();
        avg_time_bres_real += end - start;

        start = micros();
        bres_int(painter, line, true);
        end = micros();
        avg_time_bres_int += end - start;

        start = micros();
        bres_smooth(painter, line, true);
        end = micros();
        avg_time_bres_smooth += end - start;
        
        start = micros();
        wu(painter, line, true);
        end = micros();
        avg_time_wu += end - start;
    }

    avg_time_dda /= (double)measures;
    avg_time_bres_real /= (double)measures;
    avg_time_bres_int /= (double)measures;
    avg_time_bres_smooth /= (double)measures;
    avg_time_wu /= (double)measures;

    createTimeHist(avg_time_dda, avg_time_bres_real, avg_time_bres_int, avg_time_bres_smooth, avg_time_wu, "time.gp");
    system("gnuplot graphs/time.gp");
}

void createStepsGraph(const std::vector<int> &data, const std::string &filename, const std::string &graphName) 
{
    std::ofstream gpFile("graphs/" + filename);
        
    gpFile << "# Gnuplot script for plotting vector data\n\n";
    gpFile << "set xlabel 'Градус наклона (длина = 250)'\n";
    gpFile << "set ylabel 'Количество ступенек'\n";
    gpFile << "set grid\n";
    
    int plotNumber = 1;
    
    gpFile << "\n# Plot " << plotNumber << "\n";
    gpFile << "set terminal wxt " << plotNumber << " title '" << graphName << "'\n";
    
    gpFile << "plot '-' with points pointtype 7 pointsize 1 title 'Кол-во ступенек'\n";
    
    for (size_t i = 0; i < data.size(); ++i) 
    {
        double x = i * 5.0;
        gpFile << x << " " << data[i] << "\n";
    }
    gpFile << "e\n"; // Конец данных
    
    gpFile << "pause mouse close\n";
        
    gpFile.close();
}

void MainWindow::onCompareStepsBtnClicked()
{
    QPainter painter; // FILLER
    std::vector<int> dda_steps_count, bres_real_steps_count, bres_int_steps_count, bres_smooth_steps_count, wu_steps_count;

    int s = 90 / 5;

    line_t line;
    line.start = {250, 250};
    line.end = {500, 250};
    line.color = {0, 0, 0};   

    for (int j = 0; j < s; j++)
    {            
        dda_steps_count.push_back(dda(painter, line, true));
        bres_real_steps_count.push_back(bres_real(painter, line, true));
        bres_int_steps_count.push_back(bres_int(painter, line, true));
        bres_smooth_steps_count.push_back(bres_smooth(painter, line, true));
        wu_steps_count.push_back(wu(painter, line, true));

        ui->planeWidget->rotatePoint(qDegreesToRadians(5), line.end, line.start);
    }

    createStepsGraph(dda_steps_count, "dda.gp", "ЦДА");
    createStepsGraph(bres_real_steps_count, "bres_real.gp", "Брез. (real)");
    createStepsGraph(bres_int_steps_count, "bres_int.gp", "Брез. (int)");
    createStepsGraph(bres_smooth_steps_count, "bres_smooth.gp", "Брез. (smooth)");
    createStepsGraph(wu_steps_count, "wu.gp", "Ву");

    system("gnuplot graphs/dda.gp");
    system("gnuplot graphs/bres_real.gp");
    system("gnuplot graphs/bres_int.gp");
    system("gnuplot graphs/bres_smooth.gp");
    system("gnuplot graphs/wu.gp");
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
