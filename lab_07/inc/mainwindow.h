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
    void setRectColorDisplayColor(const QColor &color);
    void setResColorDisplayColor(const QColor &color);

private slots:
    void onLineColorBtnClicked();
    void onRectColorBtnClicked();
    void onResColorBtnClicked();

    void onClearScreenBtnClicked();

    void onDrawLineBtnClicked();
    void onDrawRectBtnClicked();
    
    void onCutBtnClicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif /* MAINWINDOW_H */
