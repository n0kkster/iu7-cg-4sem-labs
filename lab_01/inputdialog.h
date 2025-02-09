#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>

namespace Ui {
class InputDialog;
}

class InputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputDialog(QWidget *parent = nullptr);
    ~InputDialog();

    std::array<QPointF, 3> getTrianglePoints() const;

private slots:
    void onAddBtnClicked();

private:
    Ui::InputDialog *ui;
    std::array<QPointF, 3> trianglePoints;
};

#endif // INPUTDIALOG_H
