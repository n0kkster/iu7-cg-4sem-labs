#include <QMessageBox>
#include <QColorDialog>

#include "mainwindow.h"
#include "plane.h"

#include "out/ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->drawLineBtn, &QPushButton::clicked, this, &MainWindow::onDrawLineBtnClicked);
    connect(ui->lineColorBtn, &QPushButton::clicked, this, &MainWindow::onLineColorBtnClicked);
    connect(ui->bgColorBtn, &QPushButton::clicked, this, &MainWindow::onBgColorBtnClicked);
    connect(ui->clearScreenBtn, &QPushButton::clicked, this, &MainWindow::onClearScreenBtnClicked);

    setLineColorDisplayColor("white");
}

void MainWindow::onDrawLineBtnClicked()
{
    int xs, ys, xe, ye;
    double angle;
    bool ok;
    method_e method;

    xs = ui->xsInp->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Начальная координата по оси X должна быть вещественным числом!");
        return;
    }

    ys = ui->ysInp->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Начальная координата по оси Y должна быть вещественным числом!");
        return;
    }

    xe = ui->xeInp->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Конечная координата по оси X должна быть вещественным числом!");
        return;
    }

    ye = ui->yeInp->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Конечная координата по оси Y должна быть вещественным числом!");
        return;
    }

    method = (method_e)ui->algoDropDown->currentIndex();
    ui->planeWidget->addLine({{xs, ys}, {xe, ye}, method, ui->lineColorDisplay->palette().color(QPalette::Window)});
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
