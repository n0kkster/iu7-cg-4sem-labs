#include "mainwindow.h"
#include "inputdialog.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->addPointBtn, &QPushButton::clicked, this, &MainWindow::onAddBtnClicked);
    connect(ui->removeSelPoint, &QPushButton::clicked, this, &MainWindow::onRemoveBtnClicked);
    connect(ui->removePointsBtn, &QPushButton::clicked, this, &MainWindow::onRemoveAllBtnClicked);
    connect(ui->addTriangleBtn, &QPushButton::clicked, this, &MainWindow::onAddTriangleBtnClicked);

    connect(ui->pointsTable, &QTableWidget::cellChanged, this, &MainWindow::onCellChanged);

    connect(ui->planeWidget, &Plane::clicked, this, &MainWindow::onPlaneClicked);
}

void MainWindow::onPlaneClicked(QPointF point)
{
    QTableWidgetItem *item;

    ui->pointsTable->setRowCount(ui->planeWidget->pointsCount);

    item = new QTableWidgetItem(QString(std::to_string(ui->planeWidget->pointsCount).c_str()));
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pointsTable->setItem(ui->planeWidget->pointsCount - 1, 0, item);
    ui->pointsTable->setItem(ui->planeWidget->pointsCount - 1, 1, new QTableWidgetItem(QString(std::to_string(point.rx()).c_str())));
    ui->pointsTable->setItem(ui->planeWidget->pointsCount - 1, 2, new QTableWidgetItem(QString(std::to_string(point.ry()).c_str())));
}

void MainWindow::onAddTriangleBtnClicked()
{
    InputDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        std::array<QPointF, 3> trianglePoints = dialog.getTrianglePoints();
        ui->planeWidget->addTriangle(trianglePoints);
    }
}

void MainWindow::onAddBtnClicked()
{
    double x, y;
    bool ok;
    int points_count;
    QTableWidgetItem *item;

    qDebug() << "X: " << ui->xInput->text() << " Y: " << ui->yInput->text();

    x = ui->xInput->text().toDouble(&ok);
    if (!ok)
    {
        qDebug() << "X must be double!";
        return;
    }

    y = ui->yInput->text().toDouble(&ok);
    if (!ok)
    {
        qDebug() << "Y must be double!";
        return;
    }

    ui->pointsTable->setRowCount(++ui->planeWidget->pointsCount);

    item = new QTableWidgetItem(QString(std::to_string(ui->planeWidget->pointsCount).c_str()));
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pointsTable->setItem(ui->planeWidget->pointsCount - 1, 0, item);
    ui->pointsTable->setItem(ui->planeWidget->pointsCount - 1, 1, new QTableWidgetItem(ui->xInput->text()));
    ui->pointsTable->setItem(ui->planeWidget->pointsCount - 1, 2, new QTableWidgetItem(ui->yInput->text()));

    ui->xInput->setText("");
    ui->yInput->setText("");

    ui->planeWidget->points.append(std::pair<int, QPointF>{ui->planeWidget->pointsCount, QPointF{x, y}});
    ui->planeWidget->viewport()->update();
}

void MainWindow::onRemoveBtnClicked()
{
    QTableWidgetItem *curr = ui->pointsTable->currentItem();
    int id;

    if (curr == nullptr)
        return;

    id = ui->pointsTable->item(curr->row(), 0)->text().toInt();
    for (auto it = ui->planeWidget->points.begin(); it != ui->planeWidget->points.end();)
    {
        if (it->first == id)
            it = ui->planeWidget->points.erase(it);
        else
            ++it;
    }
    ui->planeWidget->viewport()->update();

    ui->pointsTable->removeRow(curr->row());
    ui->pointsTable->setRowCount(--ui->planeWidget->pointsCount);
}

void MainWindow::onRemoveAllBtnClicked()
{
    while (ui->planeWidget->pointsCount)
        ui->pointsTable->removeRow(ui->planeWidget->pointsCount--);

    ui->pointsTable->setRowCount(ui->planeWidget->pointsCount);
    ui->planeWidget->points.clear();
    ui->planeWidget->viewport()->update();
}

void MainWindow::onCellChanged(int row, int column)
{
    double x, y;
    int id;

    if (!(ui->pointsTable->item(row, 0) &&
          ui->pointsTable->item(row, 1) &&
          ui->pointsTable->item(row, 2)))
        return;

    id = ui->pointsTable->item(row, 0)->text().toInt();
    x = ui->pointsTable->item(row, 1)->text().toDouble();
    y = ui->pointsTable->item(row, 2)->text().toDouble();

    for (auto &p : ui->planeWidget->points)
    {
        if (p.first == id)
        {
            p.second.setX(x);
            p.second.setY(y);
            ui->planeWidget->viewport()->update();
            break;
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
