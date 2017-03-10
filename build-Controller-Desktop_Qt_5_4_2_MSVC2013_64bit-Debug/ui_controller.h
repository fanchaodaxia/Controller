/********************************************************************************
** Form generated from reading UI file 'controller.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Controller
{
public:
    QWidget *centralWidget;
    QPushButton *StopButton;
    QLabel *label;
    QLabel *label_status;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_status_2;
    QLabel *label_status_3;
    QLabel *label_status_4;
    QPushButton *ExeButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Controller)
    {
        if (Controller->objectName().isEmpty())
            Controller->setObjectName(QStringLiteral("Controller"));
        Controller->resize(400, 300);
        centralWidget = new QWidget(Controller);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        StopButton = new QPushButton(centralWidget);
        StopButton->setObjectName(QStringLiteral("StopButton"));
        StopButton->setGeometry(QRect(300, 200, 81, 31));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 91, 31));
        label_status = new QLabel(centralWidget);
        label_status->setObjectName(QStringLiteral("label_status"));
        label_status->setGeometry(QRect(110, 10, 91, 31));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(80, 60, 135, 111));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit = new QLineEdit(layoutWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        lineEdit_2 = new QLineEdit(layoutWidget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        verticalLayout->addWidget(lineEdit_2);

        lineEdit_3 = new QLineEdit(layoutWidget);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        verticalLayout->addWidget(lineEdit_3);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 60, 78, 111));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_status_2 = new QLabel(layoutWidget1);
        label_status_2->setObjectName(QStringLiteral("label_status_2"));

        verticalLayout_2->addWidget(label_status_2);

        label_status_3 = new QLabel(layoutWidget1);
        label_status_3->setObjectName(QStringLiteral("label_status_3"));

        verticalLayout_2->addWidget(label_status_3);

        label_status_4 = new QLabel(layoutWidget1);
        label_status_4->setObjectName(QStringLiteral("label_status_4"));

        verticalLayout_2->addWidget(label_status_4);

        ExeButton = new QPushButton(centralWidget);
        ExeButton->setObjectName(QStringLiteral("ExeButton"));
        ExeButton->setGeometry(QRect(190, 200, 71, 31));
        Controller->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Controller);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 26));
        Controller->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Controller);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Controller->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Controller);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Controller->setStatusBar(statusBar);

        retranslateUi(Controller);

        QMetaObject::connectSlotsByName(Controller);
    } // setupUi

    void retranslateUi(QMainWindow *Controller)
    {
        Controller->setWindowTitle(QApplication::translate("Controller", "Controller", 0));
        StopButton->setText(QApplication::translate("Controller", "Motor Stop", 0));
        label->setText(QApplication::translate("Controller", "Controller status", 0));
        label_status->setText(QApplication::translate("Controller", "status", 0));
        label_status_2->setText(QApplication::translate("Controller", "X position", 0));
        label_status_3->setText(QApplication::translate("Controller", "Y position", 0));
        label_status_4->setText(QApplication::translate("Controller", "Retract cover", 0));
        ExeButton->setText(QApplication::translate("Controller", "Execute", 0));
    } // retranslateUi

};

namespace Ui {
    class Controller: public Ui_Controller {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLLER_H
