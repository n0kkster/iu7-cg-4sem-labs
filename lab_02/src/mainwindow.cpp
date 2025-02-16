#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->applyOffsetBtn, &QPushButton::clicked, this, &MainWindow::onApplyOffsetBtnClicked);
    connect(ui->applyScaleBtn, &QPushButton::clicked, this, &MainWindow::onApplyScaleBtnClicked);
    connect(ui->applyRotationBtn, &QPushButton::clicked, this, &MainWindow::onApplyRotationBtnClicked);
    connect(ui->recoilTransformBtn, &QPushButton::clicked, ui->planeWidget, &Plane::rollbackTransformation);
}

void MainWindow::onApplyOffsetBtnClicked()
{
    double dx, dy;
    bool ok;

    dx = ui->dxOffsetInput->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Величина сдвига по оси X должна быть вещественным числом.");
        return;
    }

    dy = ui->dyOffsetInput->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Величина сдвига по оси Y должна быть вещественным числом.");
        return;
    }

    ui->planeWidget->addOffset({dx, dy});
    ui->planeWidget->viewport()->update();
}

void MainWindow::onApplyScaleBtnClicked()
{
    double cx, cy, kx, ky;
    bool ok;

    cx = ui->centerXScale->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата X центра масштабирования должна быть вещественным числом.");
        return;
    }

    cy = ui->centerYScale->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата Y центра масштабирования должна быть вещественным числом.");
        return;
    }

    kx = ui->kXScale->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Коэффициент масштабирования по оси X должен быть вещественным числом.");
        return;
    }

    ky = ui->kYScale->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Коэффициент масштабирования по оси Y должен быть вещественным числом.");
        return;
    }

    ui->planeWidget->addScale({cx, cy, kx, ky});
    ui->planeWidget->viewport()->update();
}

void MainWindow::onApplyRotationBtnClicked()
{
    double cx, cy, angle;
    bool ok;

    cx = ui->centerXRotate->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата X центра вращения должна быть вещественным числом.");
        return;
    }

    cy = ui->centerYRotate->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата Y центра вращения должна быть вещественным числом.");
        return;
    }

    angle = ui->angleRotate->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Угол вращения должен быть вещественным числом.");
        return;
    }

    ui->planeWidget->addRotation({cx, cy, qDegreesToRadians(angle)});
    ui->planeWidget->viewport()->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}
