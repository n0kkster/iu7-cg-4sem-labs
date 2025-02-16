#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "out/ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
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
    void onApplyTransformBtnClicked();

private:
    Ui::MainWindow *ui;
    void uiSetScale();
    void uiSetOffset();
    void uiSetRotation();
};
#endif /* MAINWINDOW_H */
