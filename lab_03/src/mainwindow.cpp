#include <QMessageBox>
#include <QColorDialog>

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
    line.end = {xs + length, ys};

    ui->planeWidget->rotatePoint(angle, line.end, line.start);
}

unsigned long long micros(void)
{
    struct timeval value;
    gettimeofday(&value, NULL);
    return (unsigned long long)value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
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
}

void MainWindow::onCompareStepsBtnClicked()
{
    
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
