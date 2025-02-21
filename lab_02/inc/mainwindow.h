#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "out/ui_mainwindow.h"

#define DRAW_DELAY (50 * 1000)

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
    void onApplyOffsetBtnClicked();
    void onApplyScaleBtnClicked();
    void onApplyRotationBtnClicked();

private:
    Ui::MainWindow *ui;
};
#endif /* MAINWINDOW_H */
