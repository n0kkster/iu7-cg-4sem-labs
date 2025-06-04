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
    void setShapeColorDisplayColor(const QColor &color);
    void setCutterColorDisplayColor(const QColor &color);
    void setResColorDisplayColor(const QColor &color);

private slots:
    void onShapeColorBtnClicked();
    void onCutterColorBtnClicked();
    void onResColorBtnClicked();


    void onDrawShapeVertexBtnClicked();
    void onDrawCutterVertexBtnClicked();

    void onConnectShapeBtnClicked();
    void onConnectCutterBtnClicked();

    void onClearScreenBtnClicked();
    void onCutBtnClicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif /* MAINWINDOW_H */
