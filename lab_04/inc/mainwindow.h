#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "common.h"

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
    void setLineColorDisplayColor(const QColor &color);

private slots:
    void onDrawCircleBtnClicked();
    void onDrawEllipseBtnClicked();
    
    void onLineColorBtnClicked();
    void onBgColorBtnClicked();
    
    void onDrawSpectreCircleBtnClicked();
    void onDrawSpectreEllipseBtnClicked();

    void onCompareTimeBtnClicked();
    void onClearScreenBtnClicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();  
};
#endif /* MAINWINDOW_H */
