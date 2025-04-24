#include "mainwindow.h"

#include "out/ui_mainwindow.h"
#include <sys/time.h>

#include <QColorDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->clearScreenBtn, &QPushButton::clicked, this, &MainWindow::onClearScreenBtnClicked);
    connect(ui->drawLineBtn, &QPushButton::clicked, this, &MainWindow::onDrawLineBtnClicked);
    connect(ui->doBtn, &QPushButton::clicked, this, &MainWindow::onDoBtnClicked);
    connect(ui->lineColorBtn, &QPushButton::clicked, this, &MainWindow::onLineColorBtnClicked);
    connect(ui->rectColorBtn, &QPushButton::clicked, this, &MainWindow::onRectColorBtnClicked);
    connect(ui->resColorBtn, &QPushButton::clicked, this, &MainWindow::onResColorBtnClicked);


    setLineColorDisplayColor(QColor::fromRgb(246, 240, 240));
    setRectColorDisplayColor(QColor::fromRgb(126, 172, 181));
    setResColorDisplayColor(QColor::fromRgb(201, 104, 104));
}

void MainWindow::onLineColorBtnClicked()
{
    QColor fillColor = QColorDialog::getColor();
    setLineColorDisplayColor(fillColor);
    ui->planeWidget->setLineColor(fillColor);
}

void MainWindow::onRectColorBtnClicked()
{
    QColor fillColor = QColorDialog::getColor();
    setRectColorDisplayColor(fillColor);
    ui->planeWidget->setRectColor(fillColor);
}

void MainWindow::onResColorBtnClicked()
{
    QColor fillColor = QColorDialog::getColor();
    setResColorDisplayColor(fillColor);
    ui->planeWidget->setResColor(fillColor);
}

void MainWindow::onClearScreenBtnClicked()
{
    ui->planeWidget->clearPlane();
}

void MainWindow::onDoBtnClicked()
{

}

void MainWindow::onDrawLineBtnClicked()
{
    int x, y;
    bool ok;
    int number;

    x = ui->xsInp->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата X должна быть корректным целым числом.");
        return;
    }

    y = ui->ysInp->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата Y должна быть корректным целым числом.");
        return;
    }


    ui->planeWidget->viewport()->update();

}

void MainWindow::setLineColorDisplayColor(const QColor &color)
{
    QPalette palette = ui->lineColorDisplay->palette();
    palette.setColor(QPalette::Window, color);
    ui->lineColorDisplay->setAutoFillBackground(true);
    ui->lineColorDisplay->setPalette(palette);
    ui->lineColorDisplay->show();
}

void MainWindow::setRectColorDisplayColor(const QColor &color)
{
    QPalette palette = ui->rectColorDisplay->palette();
    palette.setColor(QPalette::Window, color);
    ui->rectColorDisplay->setAutoFillBackground(true);
    ui->rectColorDisplay->setPalette(palette);
    ui->rectColorDisplay->show();
}

void MainWindow::setResColorDisplayColor(const QColor &color)
{
    QPalette palette = ui->resColorDisplay->palette();
    palette.setColor(QPalette::Window, color);
    ui->resColorDisplay->setAutoFillBackground(true);
    ui->resColorDisplay->setPalette(palette);
    ui->resColorDisplay->show();
}



MainWindow::~MainWindow() { delete ui; }
