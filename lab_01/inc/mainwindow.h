#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddBtnClicked();
    void onCellChanged(int row, int column);
    void onRemoveBtnClicked();
    void onRemoveAllBtnClicked();
    void onPlaneClicked(QPointF point);
    void onAddTriangleBtnClicked();

private:
    Ui::MainWindow *ui;
};
#endif /* MAINWINDOW_H */
