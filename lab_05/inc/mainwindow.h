#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plane.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    void setFillColorDisplayColor(const QColor &color);

private slots:
    void onFillColorBtnClicked();
    void onClearScreenBtnClicked();
    void onConnectShapeBtnClicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();  
};
#endif /* MAINWINDOW_H */
