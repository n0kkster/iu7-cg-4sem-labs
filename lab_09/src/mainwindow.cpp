#include "mainwindow.h"

#include "out/ui_mainwindow.h"
#include "plane.h"
#include <sys/time.h>

#include <QColorDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->drawShapeVertexBtn, &QPushButton::clicked, this, &MainWindow::onDrawShapeVertexBtnClicked);
    connect(ui->drawCutterVertexBtn, &QPushButton::clicked, this, &MainWindow::onDrawCutterVertexBtnClicked);

    connect(ui->shapeColorBtn, &QPushButton::clicked, this, &MainWindow::onShapeColorBtnClicked);
    connect(ui->cutterColorBtn, &QPushButton::clicked, this, &MainWindow::onCutterColorBtnClicked);
    connect(ui->resColorBtn, &QPushButton::clicked, this, &MainWindow::onResColorBtnClicked);

    connect(ui->clearScreenBtn, &QPushButton::clicked, this, &MainWindow::onClearScreenBtnClicked);
    connect(ui->cutBtn, &QPushButton::clicked, this, &MainWindow::onCutBtnClicked);

    connect(ui->connectShapeBtn, &QPushButton::clicked, this, &MainWindow::onConnectShapeBtnClicked);
    connect(ui->connectCutterBtn, &QPushButton::clicked, this, &MainWindow::onConnectCutterBtnClicked);

    setShapeColorDisplayColor(QColor::fromRgb(246, 240, 240));
    setCutterColorDisplayColor(QColor::fromRgb(126, 172, 181));
    setResColorDisplayColor(QColor::fromRgb(201, 104, 104));
}

void MainWindow::onShapeColorBtnClicked()
{
    QColor shapeColor = QColorDialog::getColor();
    setShapeColorDisplayColor(shapeColor);
    ui->planeWidget->setShapeColor(shapeColor);
}

void MainWindow::onCutterColorBtnClicked()
{
    QColor cutterColor = QColorDialog::getColor();
    setCutterColorDisplayColor(cutterColor);
    ui->planeWidget->setCutterColor(cutterColor);
}

void MainWindow::onResColorBtnClicked()
{
    QColor resColor = QColorDialog::getColor();
    setResColorDisplayColor(resColor);
    ui->planeWidget->setResColor(resColor);
}

void MainWindow::onConnectShapeBtnClicked()
{
    ui->planeWidget->connectShape();
    ui->planeWidget->viewport()->update();
}

void MainWindow::onConnectCutterBtnClicked()
{
    ui->planeWidget->connectCutter();
    ui->planeWidget->viewport()->update();
}

void MainWindow::onClearScreenBtnClicked()
{
    ui->planeWidget->clearPlane();
}

void MainWindow::onCutBtnClicked()
{
    ui->planeWidget->enableCut();
    ui->planeWidget->viewport()->update();
}

void MainWindow::onDrawShapeVertexBtnClicked()
{
    int x, y;
    bool ok;

    x = ui->xsInp->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата X вершины многоугольника должна быть корректным целым числом.");
        return;
    }

    y = ui->ysInp->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата Y вершины многоугольника должна быть корректным целым числом.");
        return;
    }

    ui->planeWidget->addShapePoint({ static_cast<double>(x), static_cast<double>(y) });

    ui->planeWidget->viewport()->update();
}

void MainWindow::onDrawCutterVertexBtnClicked()
{
    int x, y;
    bool ok;

    x = ui->xcInp->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата X вершины отсекателя должна быть корректным целым числом.");
        return;
    }

    y = ui->ycInp->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата Y вершины отсекателя должна быть корректным целым числом.");
        return;
    }

    ui->planeWidget->addCutterPoint({ static_cast<double>(x), static_cast<double>(y) });

    ui->planeWidget->viewport()->update();
}

void MainWindow::setShapeColorDisplayColor(const QColor &color)
{
    QPalette palette = ui->lineColorDisplay->palette();
    palette.setColor(QPalette::Window, color);
    ui->lineColorDisplay->setAutoFillBackground(true);
    ui->lineColorDisplay->setPalette(palette);
    ui->lineColorDisplay->show();
}

void MainWindow::setCutterColorDisplayColor(const QColor &color)
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

MainWindow::~MainWindow()
{
    delete ui;
}
