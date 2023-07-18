/********************************************************************************
** Form generated from reading UI file 'mainwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIDGET_H
#define UI_MAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEditUser;
    QLabel *label_2;
    QLineEdit *lineEditPassword;
    QPushButton *pushButtonSignIn;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButtonSignUp;
    QPushButton *pushButtonChange;

    void setupUi(QWidget *MainWidget)
    {
        if (MainWidget->objectName().isEmpty())
            MainWidget->setObjectName(QString::fromUtf8("MainWidget"));
        MainWidget->resize(300, 250);
        verticalLayout = new QVBoxLayout(MainWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(MainWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEditUser = new QLineEdit(MainWidget);
        lineEditUser->setObjectName(QString::fromUtf8("lineEditUser"));

        gridLayout->addWidget(lineEditUser, 0, 1, 1, 1);

        label_2 = new QLabel(MainWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEditPassword = new QLineEdit(MainWidget);
        lineEditPassword->setObjectName(QString::fromUtf8("lineEditPassword"));

        gridLayout->addWidget(lineEditPassword, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        pushButtonSignIn = new QPushButton(MainWidget);
        pushButtonSignIn->setObjectName(QString::fromUtf8("pushButtonSignIn"));

        verticalLayout->addWidget(pushButtonSignIn);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pushButtonSignUp = new QPushButton(MainWidget);
        pushButtonSignUp->setObjectName(QString::fromUtf8("pushButtonSignUp"));

        horizontalLayout_3->addWidget(pushButtonSignUp);

        pushButtonChange = new QPushButton(MainWidget);
        pushButtonChange->setObjectName(QString::fromUtf8("pushButtonChange"));

        horizontalLayout_3->addWidget(pushButtonChange);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(MainWidget);

        QMetaObject::connectSlotsByName(MainWidget);
    } // setupUi

    void retranslateUi(QWidget *MainWidget)
    {
        MainWidget->setWindowTitle(QCoreApplication::translate("MainWidget", "MainWidget", nullptr));
        label->setText(QCoreApplication::translate("MainWidget", "User", nullptr));
        label_2->setText(QCoreApplication::translate("MainWidget", "Password", nullptr));
        pushButtonSignIn->setText(QCoreApplication::translate("MainWidget", "Sign in", nullptr));
        pushButtonSignUp->setText(QCoreApplication::translate("MainWidget", "Sign up", nullptr));
        pushButtonChange->setText(QCoreApplication::translate("MainWidget", "Change Password", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWidget: public Ui_MainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIDGET_H
