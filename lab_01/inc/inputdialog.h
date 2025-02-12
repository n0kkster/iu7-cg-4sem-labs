#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class InputDialog;
}

class InputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputDialog(QWidget *parent = nullptr);
    ~InputDialog();

    std::array<QPointF, 3> getTrianglePoints() const { return trianglePoints; };

private slots:
    void onAddBtnClicked();

private:
    Ui::InputDialog *ui;
    std::array<QPointF, 3> trianglePoints;

    bool isTrinagleSingular() { return ((trianglePoints[1].x() - trianglePoints[0].x()) 
        * (trianglePoints[2].y() - trianglePoints[0].y()) == (trianglePoints[2].x() - trianglePoints[0].x()) 
        * (trianglePoints[1].y() - trianglePoints[0].y())); };
};

#endif // INPUTDIALOG_H
