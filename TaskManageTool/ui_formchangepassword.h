/********************************************************************************
** Form generated from reading UI file 'formchangepassword.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMCHANGEPASSWORD_H
#define UI_FORMCHANGEPASSWORD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormChangePassword
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEditOldPassword;
    QLabel *label_2;
    QLabel *label_4;
    QLineEdit *lineEditConfirm;
    QLineEdit *lineEditNewPassword;
    QLabel *label_3;
    QLineEdit *lineEditUser;
    QPushButton *pushButtonOK;

    void setupUi(QWidget *FormChangePassword)
    {
        if (FormChangePassword->objectName().isEmpty())
            FormChangePassword->setObjectName(QString::fromUtf8("FormChangePassword"));
        FormChangePassword->resize(280, 292);
        gridLayout = new QGridLayout(FormChangePassword);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(FormChangePassword);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEditOldPassword = new QLineEdit(FormChangePassword);
        lineEditOldPassword->setObjectName(QString::fromUtf8("lineEditOldPassword"));

        gridLayout->addWidget(lineEditOldPassword, 1, 2, 1, 1);

        label_2 = new QLabel(FormChangePassword);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 2);

        label_4 = new QLabel(FormChangePassword);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        lineEditConfirm = new QLineEdit(FormChangePassword);
        lineEditConfirm->setObjectName(QString::fromUtf8("lineEditConfirm"));

        gridLayout->addWidget(lineEditConfirm, 3, 2, 1, 1);

        lineEditNewPassword = new QLineEdit(FormChangePassword);
        lineEditNewPassword->setObjectName(QString::fromUtf8("lineEditNewPassword"));

        gridLayout->addWidget(lineEditNewPassword, 2, 2, 1, 1);

        label_3 = new QLabel(FormChangePassword);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 2);

        lineEditUser = new QLineEdit(FormChangePassword);
        lineEditUser->setObjectName(QString::fromUtf8("lineEditUser"));

        gridLayout->addWidget(lineEditUser, 0, 2, 1, 1);

        pushButtonOK = new QPushButton(FormChangePassword);
        pushButtonOK->setObjectName(QString::fromUtf8("pushButtonOK"));

        gridLayout->addWidget(pushButtonOK, 4, 0, 1, 3);


        retranslateUi(FormChangePassword);

        QMetaObject::connectSlotsByName(FormChangePassword);
    } // setupUi

    void retranslateUi(QWidget *FormChangePassword)
    {
        FormChangePassword->setWindowTitle(QCoreApplication::translate("FormChangePassword", "Form", nullptr));
        label->setText(QCoreApplication::translate("FormChangePassword", "User", nullptr));
        label_2->setText(QCoreApplication::translate("FormChangePassword", "Old Password", nullptr));
        label_4->setText(QCoreApplication::translate("FormChangePassword", "Comfirm", nullptr));
        label_3->setText(QCoreApplication::translate("FormChangePassword", "New Password", nullptr));
        pushButtonOK->setText(QCoreApplication::translate("FormChangePassword", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormChangePassword: public Ui_FormChangePassword {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMCHANGEPASSWORD_H
