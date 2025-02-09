/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>
#include "../../../plane.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *MainWidget;
    Plane *planeWidget;
    QTableWidget *pointsTable;
    QLineEdit *xInput;
    QLineEdit *yInput;
    QPushButton *addPointBtn;
    QPushButton *removePointsBtn;
    QPushButton *solveBtn;
    QPushButton *removeSelPoint;
    QPushButton *addPointBtn_2;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1100, 750);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1100, 750));
        MainWindow->setMaximumSize(QSize(1100, 750));
        QPalette palette;
        QBrush brush(QColor(216, 217, 218, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(39, 40, 41, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Light, brush);
        QBrush brush2(QColor(251, 251, 251, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        QBrush brush3(QColor(23, 23, 23, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush3);
        QBrush brush4(QColor(146, 136, 248, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Highlight, brush4);
        QBrush brush5(QColor(68, 119, 206, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Link, brush5);
        QBrush brush6(QColor(200, 142, 167, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::LinkVisited, brush6);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush1);
        QBrush brush7(QColor(216, 217, 218, 128));
        brush7.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Active, QPalette::PlaceholderText, brush7);
#endif
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Link, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush1);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush7);
#endif
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Link, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush7);
#endif
        MainWindow->setPalette(palette);
        MainWindow->setTabShape(QTabWidget::Triangular);
        MainWidget = new QWidget(MainWindow);
        MainWidget->setObjectName("MainWidget");
        MainWidget->setMinimumSize(QSize(1100, 750));
        MainWidget->setMaximumSize(QSize(1100, 750));
        planeWidget = new Plane(MainWidget);
        planeWidget->setObjectName("planeWidget");
        planeWidget->setGeometry(QRect(10, 10, 800, 700));
        pointsTable = new QTableWidget(MainWidget);
        if (pointsTable->columnCount() < 3)
            pointsTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        pointsTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        pointsTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        pointsTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        pointsTable->setObjectName("pointsTable");
        pointsTable->setGeometry(QRect(820, 170, 270, 360));
        pointsTable->setAutoFillBackground(false);
        pointsTable->setFrameShape(QFrame::NoFrame);
        pointsTable->setFrameShadow(QFrame::Sunken);
        pointsTable->setLineWidth(1);
        pointsTable->setShowGrid(true);
        pointsTable->setRowCount(0);
        pointsTable->setColumnCount(3);
        pointsTable->horizontalHeader()->setVisible(true);
        pointsTable->horizontalHeader()->setCascadingSectionResizes(false);
        pointsTable->horizontalHeader()->setDefaultSectionSize(90);
        pointsTable->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        pointsTable->horizontalHeader()->setStretchLastSection(false);
        xInput = new QLineEdit(MainWidget);
        xInput->setObjectName("xInput");
        xInput->setGeometry(QRect(820, 10, 270, 30));
        yInput = new QLineEdit(MainWidget);
        yInput->setObjectName("yInput");
        yInput->setGeometry(QRect(820, 50, 270, 30));
        addPointBtn = new QPushButton(MainWidget);
        addPointBtn->setObjectName("addPointBtn");
        addPointBtn->setGeometry(QRect(820, 90, 270, 30));
        QFont font;
        font.setFamilies({QString::fromUtf8("Sans Serif")});
        font.setPointSize(11);
        addPointBtn->setFont(font);
        addPointBtn->setAutoFillBackground(true);
        addPointBtn->setFlat(true);
        removePointsBtn = new QPushButton(MainWidget);
        removePointsBtn->setObjectName("removePointsBtn");
        removePointsBtn->setGeometry(QRect(820, 580, 270, 30));
        QFont font1;
        font1.setPointSize(11);
        removePointsBtn->setFont(font1);
        removePointsBtn->setAutoFillBackground(true);
        removePointsBtn->setFlat(true);
        solveBtn = new QPushButton(MainWidget);
        solveBtn->setObjectName("solveBtn");
        solveBtn->setGeometry(QRect(820, 620, 270, 90));
        QFont font2;
        font2.setPointSize(16);
        solveBtn->setFont(font2);
        solveBtn->setAutoFillBackground(true);
        solveBtn->setFlat(true);
        removeSelPoint = new QPushButton(MainWidget);
        removeSelPoint->setObjectName("removeSelPoint");
        removeSelPoint->setGeometry(QRect(820, 540, 270, 30));
        removeSelPoint->setFont(font1);
        removeSelPoint->setAutoFillBackground(true);
        removeSelPoint->setFlat(true);
        addPointBtn_2 = new QPushButton(MainWidget);
        addPointBtn_2->setObjectName("addPointBtn_2");
        addPointBtn_2->setGeometry(QRect(820, 130, 270, 30));
        addPointBtn_2->setFont(font1);
        addPointBtn_2->setAutoFillBackground(true);
        addPointBtn_2->setFlat(true);
        MainWindow->setCentralWidget(MainWidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        addPointBtn->setDefault(false);
        removePointsBtn->setDefault(false);
        solveBtn->setDefault(false);
        removeSelPoint->setDefault(false);
        addPointBtn_2->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\320\233\320\2401 \320\277\320\276 \320\232\320\223. \320\224\320\270\320\262\320\260\320\265\320\262 \320\230\320\2437-42", nullptr));
        QTableWidgetItem *___qtablewidgetitem = pointsTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "#", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = pointsTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = pointsTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        xInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\276\321\200\320\264\320\270\320\275\320\260\321\202\320\260 X", nullptr));
        yInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\276\321\200\320\264\320\270\320\275\320\260\321\202\320\260 Y", nullptr));
        addPointBtn->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\202\320\276\321\207\320\272\321\203", nullptr));
        removePointsBtn->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\262\321\201\320\265 \321\202\320\276\321\207\320\272\320\270", nullptr));
        solveBtn->setText(QCoreApplication::translate("MainWindow", "\320\240\320\265\321\210\320\270\321\202\321\214 \320\267\320\260\320\264\320\260\321\207\321\203", nullptr));
        removeSelPoint->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\262\321\213\320\261\321\200\320\260\320\275\320\275\321\203\321\216 \321\202\320\276\321\207\320\272\321\203", nullptr));
        addPointBtn_2->setText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\321\201\321\202\320\270 \321\202\321\200\320\265\321\203\320\263\320\276\320\273\321\214\320\275\320\270\320\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
