/********************************************************************************
** Form generated from reading UI file 'formaddrule.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMADDRULE_H
#define UI_FORMADDRULE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormAddRule
{
public:
    QGridLayout *gridLayout;
    QComboBox *comboBoxCmd;
    QLineEdit *lineEditPath;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonOK;
    QLabel *label;
    QLabel *label_5;
    QComboBox *comboBoxRe;
    QLabel *label_3;
    QDateTimeEdit *dateTimeEditRemind;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_4;
    QPushButton *pushButton;
    QLineEdit *lineEditUsername;

    void setupUi(QWidget *FormAddRule)
    {
        if (FormAddRule->objectName().isEmpty())
            FormAddRule->setObjectName(QString::fromUtf8("FormAddRule"));
        FormAddRule->resize(271, 363);
        gridLayout = new QGridLayout(FormAddRule);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        comboBoxCmd = new QComboBox(FormAddRule);
        comboBoxCmd->addItem(QString());
        comboBoxCmd->addItem(QString());
        comboBoxCmd->addItem(QString());
        comboBoxCmd->addItem(QString());
        comboBoxCmd->addItem(QString());
        comboBoxCmd->addItem(QString());
        comboBoxCmd->addItem(QString());
        comboBoxCmd->addItem(QString());
        comboBoxCmd->addItem(QString());
        comboBoxCmd->addItem(QString());
        comboBoxCmd->addItem(QString());
        comboBoxCmd->addItem(QString());
        comboBoxCmd->addItem(QString());
        comboBoxCmd->setObjectName(QString::fromUtf8("comboBoxCmd"));

        gridLayout->addWidget(comboBoxCmd, 2, 2, 1, 1);

        lineEditPath = new QLineEdit(FormAddRule);
        lineEditPath->setObjectName(QString::fromUtf8("lineEditPath"));

        gridLayout->addWidget(lineEditPath, 4, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(69, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 8, 0, 1, 1);

        pushButtonOK = new QPushButton(FormAddRule);
        pushButtonOK->setObjectName(QString::fromUtf8("pushButtonOK"));

        gridLayout->addWidget(pushButtonOK, 8, 2, 1, 1);

        label = new QLabel(FormAddRule);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_5 = new QLabel(FormAddRule);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 5, 0, 1, 1);

        comboBoxRe = new QComboBox(FormAddRule);
        comboBoxRe->addItem(QString());
        comboBoxRe->addItem(QString());
        comboBoxRe->addItem(QString());
        comboBoxRe->setObjectName(QString::fromUtf8("comboBoxRe"));

        gridLayout->addWidget(comboBoxRe, 3, 2, 1, 1);

        label_3 = new QLabel(FormAddRule);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        dateTimeEditRemind = new QDateTimeEdit(FormAddRule);
        dateTimeEditRemind->setObjectName(QString::fromUtf8("dateTimeEditRemind"));

        gridLayout->addWidget(dateTimeEditRemind, 5, 2, 1, 2);

        label_2 = new QLabel(FormAddRule);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(69, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 8, 3, 1, 1);

        label_4 = new QLabel(FormAddRule);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        pushButton = new QPushButton(FormAddRule);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 6, 2, 1, 1);

        lineEditUsername = new QLineEdit(FormAddRule);
        lineEditUsername->setObjectName(QString::fromUtf8("lineEditUsername"));

        gridLayout->addWidget(lineEditUsername, 0, 2, 1, 1);


        retranslateUi(FormAddRule);

        QMetaObject::connectSlotsByName(FormAddRule);
    } // setupUi

    void retranslateUi(QWidget *FormAddRule)
    {
        FormAddRule->setWindowTitle(QCoreApplication::translate("FormAddRule", "Form", nullptr));
        comboBoxCmd->setItemText(0, QCoreApplication::translate("FormAddRule", "All", nullptr));
        comboBoxCmd->setItemText(1, QCoreApplication::translate("FormAddRule", "unlinkat", nullptr));
        comboBoxCmd->setItemText(2, QCoreApplication::translate("FormAddRule", "openat", nullptr));
        comboBoxCmd->setItemText(3, QCoreApplication::translate("FormAddRule", "close", nullptr));
        comboBoxCmd->setItemText(4, QCoreApplication::translate("FormAddRule", "read", nullptr));
        comboBoxCmd->setItemText(5, QCoreApplication::translate("FormAddRule", "exec", nullptr));
        comboBoxCmd->setItemText(6, QCoreApplication::translate("FormAddRule", "connect", nullptr));
        comboBoxCmd->setItemText(7, QCoreApplication::translate("FormAddRule", "socket", nullptr));
        comboBoxCmd->setItemText(8, QCoreApplication::translate("FormAddRule", "module", nullptr));
        comboBoxCmd->setItemText(9, QCoreApplication::translate("FormAddRule", "sendto", nullptr));
        comboBoxCmd->setItemText(10, QCoreApplication::translate("FormAddRule", "recvfrom", nullptr));
        comboBoxCmd->setItemText(11, QCoreApplication::translate("FormAddRule", "mount", nullptr));
        comboBoxCmd->setItemText(12, QCoreApplication::translate("FormAddRule", "bind", nullptr));

        pushButtonOK->setText(QCoreApplication::translate("FormAddRule", "OK", nullptr));
        label->setText(QCoreApplication::translate("FormAddRule", "Username", nullptr));
        label_5->setText(QCoreApplication::translate("FormAddRule", "logtime", nullptr));
        comboBoxRe->setItemText(0, QCoreApplication::translate("FormAddRule", "All", nullptr));
        comboBoxRe->setItemText(1, QCoreApplication::translate("FormAddRule", "Success", nullptr));
        comboBoxRe->setItemText(2, QCoreApplication::translate("FormAddRule", "Fail", nullptr));

        label_3->setText(QCoreApplication::translate("FormAddRule", "result", nullptr));
        label_2->setText(QCoreApplication::translate("FormAddRule", "Audittype", nullptr));
        label_4->setText(QCoreApplication::translate("FormAddRule", "file_path", nullptr));
        pushButton->setText(QCoreApplication::translate("FormAddRule", "All time", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormAddRule: public Ui_FormAddRule {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMADDRULE_H
