/********************************************************************************
** Form generated from reading UI file 'formcreatenewuser.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMCREATENEWUSER_H
#define UI_FORMCREATENEWUSER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormCreateNewUser
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEditUser;
    QLabel *label_2;
    QLineEdit *lineEditPassword;
    QLabel *label_3;
    QLineEdit *lineEditConfirm;
    QPushButton *pushButtonOK;

    void setupUi(QWidget *FormCreateNewUser)
    {
        if (FormCreateNewUser->objectName().isEmpty())
            FormCreateNewUser->setObjectName(QString::fromUtf8("FormCreateNewUser"));
        FormCreateNewUser->resize(266, 300);
        gridLayout = new QGridLayout(FormCreateNewUser);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(FormCreateNewUser);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(70, 0));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEditUser = new QLineEdit(FormCreateNewUser);
        lineEditUser->setObjectName(QString::fromUtf8("lineEditUser"));

        gridLayout->addWidget(lineEditUser, 0, 1, 1, 1);

        label_2 = new QLabel(FormCreateNewUser);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(70, 0));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEditPassword = new QLineEdit(FormCreateNewUser);
        lineEditPassword->setObjectName(QString::fromUtf8("lineEditPassword"));

        gridLayout->addWidget(lineEditPassword, 1, 1, 1, 1);

        label_3 = new QLabel(FormCreateNewUser);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(70, 0));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        lineEditConfirm = new QLineEdit(FormCreateNewUser);
        lineEditConfirm->setObjectName(QString::fromUtf8("lineEditConfirm"));

        gridLayout->addWidget(lineEditConfirm, 2, 1, 1, 1);

        pushButtonOK = new QPushButton(FormCreateNewUser);
        pushButtonOK->setObjectName(QString::fromUtf8("pushButtonOK"));

        gridLayout->addWidget(pushButtonOK, 3, 0, 1, 2);


        retranslateUi(FormCreateNewUser);

        QMetaObject::connectSlotsByName(FormCreateNewUser);
    } // setupUi

    void retranslateUi(QWidget *FormCreateNewUser)
    {
        FormCreateNewUser->setWindowTitle(QCoreApplication::translate("FormCreateNewUser", "Form", nullptr));
        label->setText(QCoreApplication::translate("FormCreateNewUser", "User", nullptr));
        label_2->setText(QCoreApplication::translate("FormCreateNewUser", "Password", nullptr));
        label_3->setText(QCoreApplication::translate("FormCreateNewUser", "Comfirm", nullptr));
        pushButtonOK->setText(QCoreApplication::translate("FormCreateNewUser", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormCreateNewUser: public Ui_FormCreateNewUser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMCREATENEWUSER_H
