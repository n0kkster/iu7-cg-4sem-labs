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
    connect(ui->planeWidget, &Plane::shapeFinished, this, &MainWindow::onPlaneShapeFinished);

    setFillColorDisplayColor("white");
}

void MainWindow::onFillColorBtnClicked()
{   
    QColor lineColor = QColorDialog::getColor();
    setFillColorDisplayColor(lineColor);
}

void MainWindow::onClearScreenBtnClicked()
{
    ui->tableWidget->setRowCount(0);
    ui->planeWidget->clearPlane();
}

QTableWidgetItem *getCenteredItem(const QString &data)
{
    QTableWidgetItem *w = new QTableWidgetItem(data);
    w->setTextAlignment(Qt::AlignHCenter);
    return w;
}

#define SET_TABLE_LAST_ROW(table, c1, c2, c3) { \
    table->insertRow(table->rowCount()); \
    table->setItem(table->rowCount() - 1, 0, getCenteredItem(c1)); \
    table->setItem(table->rowCount() - 1, 1, getCenteredItem(c2)); \
    table->setItem(table->rowCount() - 1, 2, getCenteredItem(c3)); \
}

void MainWindow::onConnectShapeBtnClicked()
{
    SET_TABLE_LAST_ROW(ui->tableWidget, "=========", "=========", "=========");
    ui->planeWidget->finishShapeEntering();
}

void MainWindow::onPlaneShapeFinished()
{
    SET_TABLE_LAST_ROW(ui->tableWidget, "=========", "=========", "=========");
}

void MainWindow::onPlaneClicked(const QPoint &pos)
{
    int number = ui->planeWidget->getTotalPointsCount();

    SET_TABLE_LAST_ROW(ui->tableWidget, QString::number(number), QString::number(pos.x()), QString::number(pos.y()));
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

    if (!ui->planeWidget->addVertex({x, y}))
        return;

    ui->planeWidget->viewport()->update();

    number = ui->planeWidget->getTotalPointsCount();

    SET_TABLE_LAST_ROW(ui->tableWidget, QString::number(number), QString::number(x), QString::number(y));
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
