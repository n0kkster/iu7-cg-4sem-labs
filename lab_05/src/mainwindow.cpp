#include <QMessageBox>
#include <QColorDialog>

#include <fstream>
#include <iostream>

#include <random>
#include <sys/time.h>

#include "mainwindow.h"

#include "out/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->fillColorBtn, &QPushButton::clicked, this, &MainWindow::onFillColorBtnClicked);
    connect(ui->clearScreenBtn, &QPushButton::clicked, this, &MainWindow::onClearScreenBtnClicked);
    connect(ui->connectShapeBtn, &QPushButton::clicked, this, &MainWindow::onConnectShapeBtnClicked);
    connect(ui->drawPointBtn, &QPushButton::clicked, this, &MainWindow::onDrawPointBtnClicked);

    connect(ui->planeWidget, &Plane::clicked, this, &MainWindow::onPlaneClicked);

    setFillColorDisplayColor("white");
}

void MainWindow::onFillColorBtnClicked()
{   
    QColor lineColor = QColorDialog::getColor();
    setFillColorDisplayColor(lineColor);
}

void MainWindow::onClearScreenBtnClicked()
{
    ui->planeWidget->clearPlane();
}

void MainWindow::onConnectShapeBtnClicked()
{
    ui->planeWidget->finishShapeEntering();
}

void MainWindow::onPlaneClicked(const QPoint &pos)
{
    int number = ui->planeWidget->getTotalPointsCount();
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(number)));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(pos.x())));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(pos.y())));
}

void MainWindow::onDrawPointBtnClicked()
{
    int x, y;
    bool ok;
    int number;

    x = ui->xInp->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата X должна быть корректным целым числом.");
        return;
    }

    y = ui->yInp->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата Y должна быть корректным целым числом.");
        return;
    }

    ui->planeWidget->addVertex({x, y});
    ui->planeWidget->viewport()->update();

    number = ui->planeWidget->getTotalPointsCount();

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(number)));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(x)));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(y)));
}

unsigned long long micros(void)
{
    struct timeval value;
    gettimeofday(&value, NULL);
    return (unsigned long long)value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}

void MainWindow::setFillColorDisplayColor(const QColor &color)
{
    QPalette palette = ui->fillColorDisplay->palette();
    palette.setColor(QPalette::Window, color);
    ui->fillColorDisplay->setAutoFillBackground(true);
    ui->fillColorDisplay->setPalette(palette);
    ui->fillColorDisplay->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
