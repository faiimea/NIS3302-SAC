/********************************************************************************
** Form generated from reading UI file 'formmenu.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMMENU_H
#define UI_FORMMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormMenu
{
public:
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;
    QPushButton *pushButtonAdd;
    QPushButton *pushButtonDelete;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QWidget *FormMenu)
    {
        if (FormMenu->objectName().isEmpty())
            FormMenu->setObjectName(QString::fromUtf8("FormMenu"));
        FormMenu->resize(1227, 487);
        gridLayout = new QGridLayout(FormMenu);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableWidget = new QTableWidget(FormMenu);
        if (tableWidget->columnCount() < 12)
            tableWidget->setColumnCount(12);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(10, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(11, __qtablewidgetitem11);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        gridLayout->addWidget(tableWidget, 8, 0, 1, 2);

        pushButtonAdd = new QPushButton(FormMenu);
        pushButtonAdd->setObjectName(QString::fromUtf8("pushButtonAdd"));
        pushButtonAdd->setMaximumSize(QSize(16777211, 16777215));

        gridLayout->addWidget(pushButtonAdd, 0, 0, 1, 1);

        pushButtonDelete = new QPushButton(FormMenu);
        pushButtonDelete->setObjectName(QString::fromUtf8("pushButtonDelete"));

        gridLayout->addWidget(pushButtonDelete, 0, 1, 1, 1);

        pushButton_2 = new QPushButton(FormMenu);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 4, 0, 1, 1);

        pushButton = new QPushButton(FormMenu);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 4, 1, 1, 1);


        retranslateUi(FormMenu);

        QMetaObject::connectSlotsByName(FormMenu);
    } // setupUi

    void retranslateUi(QWidget *FormMenu)
    {
        FormMenu->setWindowTitle(QCoreApplication::translate("FormMenu", "Form", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("FormMenu", "Id", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("FormMenu", "Username", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("FormMenu", "Uid", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("FormMenu", "Audittype", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("FormMenu", "pid", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("FormMenu", "Filepath", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("FormMenu", "Buffer", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("FormMenu", "Logtime", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("FormMenu", "Result", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("FormMenu", "Arg1", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget->horizontalHeaderItem(10);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("FormMenu", "Arg2", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget->horizontalHeaderItem(11);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("FormMenu", "Arg3", nullptr));
        pushButtonAdd->setText(QCoreApplication::translate("FormMenu", "search", nullptr));
        pushButtonDelete->setText(QCoreApplication::translate("FormMenu", "delete ", nullptr));
        pushButton_2->setText(QCoreApplication::translate("FormMenu", "reset", nullptr));
        pushButton->setText(QCoreApplication::translate("FormMenu", "pause ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormMenu: public Ui_FormMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMMENU_H
