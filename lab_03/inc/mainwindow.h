#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void onClearScreenBtnClicked();

private slots:
    void onDrawLineBtnClicked();
    void onLineColorBtnClicked();
    void onBgColorBtnClicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();  
};
#endif /* MAINWINDOW_H */
