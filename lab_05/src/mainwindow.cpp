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
