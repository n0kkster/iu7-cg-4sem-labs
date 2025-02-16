#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->applyAllBtn, &QPushButton::clicked, this, &MainWindow::onApplyTransformBtnClicked);
}

void MainWindow::onApplyTransformBtnClicked()
{
    double dx, dy, cxS, cyS, kx, ky, angle, cxR, cyR;
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

    cxS = ui->centerXScale->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата X центра масштабирования должна быть вещественным числом.");
        return;
    }

    cyS = ui->centerYScale->text().toDouble(&ok);
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

    cxR = ui->centerXRotate->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата X центра вращения должна быть вещественным числом.");
        return;
    }

    cyR = ui->centerYRotate->text().toDouble(&ok);
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

    ui->planeWidget->addOffset(dx, dy);
    ui->planeWidget->addScale(cxS, cyS, kx, ky);
    ui->planeWidget->addRotation(cxR, cyR, angle);
    ui->planeWidget->viewport()->update();
}

void MainWindow::uiSetOffset()
{

}


MainWindow::~MainWindow()
{
    delete ui;
}
