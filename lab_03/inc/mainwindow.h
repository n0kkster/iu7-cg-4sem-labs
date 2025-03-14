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
    void setLineColorDisplayColor(const QColor &color);
    void genRandomLine(line_t &line, const int length);

private slots:
    void onDrawLineBtnClicked();
    void onLineColorBtnClicked();
    void onBgColorBtnClicked();
    void onDrawSpectreBtnClicked();
    void onCompareTimeBtnClicked();
    void onCompareStepsBtnClicked();
    void onClearScreenBtnClicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();  
};
#endif /* MAINWINDOW_H */
