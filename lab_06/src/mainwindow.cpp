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

    connect(ui->fillColorBtn, &QPushButton::clicked, this, &MainWindow::onFillColorBtnClicked);
    connect(ui->clearScreenBtn, &QPushButton::clicked, this, &MainWindow::onClearScreenBtnClicked);
    connect(ui->connectShapeBtn, &QPushButton::clicked, this, &MainWindow::onConnectShapeBtnClicked);
    connect(ui->drawPointBtn, &QPushButton::clicked, this, &MainWindow::onDrawPointBtnClicked);
    connect(ui->fillBtn, &QPushButton::clicked, this, &MainWindow::onFillBtnClicked);
    connect(ui->enableDelayBtn, &QPushButton::clicked, this, &MainWindow::onEnableDelayBtnClicked);
    connect(ui->drawCircleBtn, &QPushButton::clicked, this, &MainWindow::onDrawCircleBtnClicked);
    connect(ui->drawEllipseBtn, &QPushButton::clicked, this, &MainWindow::onDrawEllipseBtnClicked);
    connect(ui->drawSeedBtn, &QPushButton::clicked, this, &MainWindow::onDrawSeedBtnClicked);

    connect(ui->planeWidget, &Plane::clicked, this, &MainWindow::onPlaneClicked);
    connect(ui->planeWidget, &Plane::shapeFinished, this, &MainWindow::onPlaneShapeFinished);

    setFillColorDisplayColor("white");
}

void MainWindow::onFillColorBtnClicked()
{
    QColor fillColor = QColorDialog::getColor();
    setFillColorDisplayColor(fillColor);
    ui->planeWidget->setFillColor(fillColor);
}

void MainWindow::onClearScreenBtnClicked()
{
    ui->tableWidget->setRowCount(0);
    ui->planeWidget->clearPlane();
}

void MainWindow::onFillBtnClicked()
{
    ui->planeWidget->enableFill();
    if (ui->planeWidget->isDelayEnabled())
        ui->planeWidget->fillSlowed();
    else
        ui->planeWidget->viewport()->update();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this,
            [=, this]()
            {
                if (ui->planeWidget->isReady())
                {
                    QMessageBox::information(this, "Время заполнения",
                                             QString("Время заполнения составило ")
                                                 + QString::number(ui->planeWidget->getFillTime())
                                                 + QString(" мс."));
                    ui->planeWidget->resetSeeds();
                    timer->stop();
                    timer->deleteLater();
                }
            });
    timer->start(10);
}

QTableWidgetItem *getCenteredItem(const QString &data)
{
    QTableWidgetItem *w = new QTableWidgetItem(data);
    w->setTextAlignment(Qt::AlignHCenter);
    return w;
}

#define SET_TABLE_LAST_ROW(table, c1, c2, c3)                          \
    {                                                                  \
        table->insertRow(table->rowCount());                           \
        table->setItem(table->rowCount() - 1, 0, getCenteredItem(c1)); \
        table->setItem(table->rowCount() - 1, 1, getCenteredItem(c2)); \
        table->setItem(table->rowCount() - 1, 2, getCenteredItem(c3)); \
    }

void MainWindow::onConnectShapeBtnClicked()
{
    SET_TABLE_LAST_ROW(ui->tableWidget, "=========", "=========", "=========");
    ui->planeWidget->finishShapeEntering();
}

void MainWindow::onEnableDelayBtnClicked()
{
    if (!ui->planeWidget->isDelayEnabled())
    {
        ui->planeWidget->enableDelay();
        ui->enableDelayBtn->setText("Выключить задержку");
    }
    else
    {
        ui->planeWidget->disableDelay();
        ui->enableDelayBtn->setText("Включить задержку");
    }
}

void MainWindow::onPlaneShapeFinished()
{
    SET_TABLE_LAST_ROW(ui->tableWidget, "=========", "=========", "=========");
}

void MainWindow::onPlaneClicked(const QPoint &pos)
{
    int number = ui->planeWidget->getTotalPointsCount();

    SET_TABLE_LAST_ROW(ui->tableWidget, QString::number(number), QString::number(pos.x()),
                       QString::number(pos.y()));
}

void MainWindow::onDrawCircleBtnClicked()
{
    double xc, yc, radius;
    bool ok;

    xc = ui->xcInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата X центра должна быть вещественным числом!");
        return;
    }

    yc = ui->ycInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата Y центра должна быть вещественным числом!");
        return;
    }

    radius = ui->circleRInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Радиус должен быть вещественным числом!");
        return;
    }

    ui->planeWidget->addCircle({
        { xc, yc },
        radius
    });
    ui->planeWidget->viewport()->update();
}

void MainWindow::onDrawEllipseBtnClicked()
{
    double xc, yc, rx, ry;
    bool ok;

    xc = ui->xcInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата X центра должна быть вещественным числом!");
        return;
    }

    yc = ui->ycInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Координата Y центра должна быть вещественным числом!");
        return;
    }

    rx = ui->ellipseRxInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Радиус по оси X должен быть вещественным числом!");
        return;
    }

    ry = ui->ellipseRyInp->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Радиус по оси Y должен быть вещественным числом!");
        return;
    }

    ui->planeWidget->addEllipse({
        { xc, yc },
        rx, ry
    });
    ui->planeWidget->viewport()->update();
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

    if (!ui->planeWidget->addVertex({ x, y }))
        return;

    ui->planeWidget->viewport()->update();

    number = ui->planeWidget->getTotalPointsCount();

    SET_TABLE_LAST_ROW(ui->tableWidget, QString::number(number), QString::number(x), QString::number(y));
}

void MainWindow::onDrawSeedBtnClicked()
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

    ui->planeWidget->addSeed({ x, y });
    ui->planeWidget->viewport()->update();
}


void MainWindow::setFillColorDisplayColor(const QColor &color)
{
    QPalette palette = ui->fillColorDisplay->palette();
    palette.setColor(QPalette::Window, color);
    ui->fillColorDisplay->setAutoFillBackground(true);
    ui->fillColorDisplay->setPalette(palette);
    ui->fillColorDisplay->show();
}

MainWindow::~MainWindow() { delete ui; }
