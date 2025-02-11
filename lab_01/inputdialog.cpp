#include "inputdialog.h"
#include "ui_inputdialog.h"

InputDialog::InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDialog)
{
    ui->setupUi(this);
    connect(ui->addTriangleBtn, &QPushButton::clicked, this, &InputDialog::onAddBtnClicked);
}

void InputDialog::onAddBtnClicked()
{
    trianglePoints[0] = QPointF{ui->x1Input->text().toDouble(), ui->y1Input->text().toDouble()};
    trianglePoints[1] = QPointF{ui->x2Input->text().toDouble(), ui->y2Input->text().toDouble()};
    trianglePoints[2] = QPointF{ui->x3Input->text().toDouble(), ui->y3Input->text().toDouble()};

    if (isTrinagleSingular())
    {
        QMessageBox::critical(this, "Ошибка", "Треугольник вырожден! Введите другие координаты");
        return;
    }

    accept();
}

InputDialog::~InputDialog()
{
    delete ui;
}
