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
    QPushButton *pushButtonDelete;
    QPushButton *pushButtonAdd;
    QTableWidget *tableWidget;

    void setupUi(QWidget *FormMenu)
    {
        if (FormMenu->objectName().isEmpty())
            FormMenu->setObjectName(QString::fromUtf8("FormMenu"));
        FormMenu->resize(722, 286);
        gridLayout = new QGridLayout(FormMenu);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButtonDelete = new QPushButton(FormMenu);
        pushButtonDelete->setObjectName(QString::fromUtf8("pushButtonDelete"));

        gridLayout->addWidget(pushButtonDelete, 2, 1, 1, 1);

        pushButtonAdd = new QPushButton(FormMenu);
        pushButtonAdd->setObjectName(QString::fromUtf8("pushButtonAdd"));

        gridLayout->addWidget(pushButtonAdd, 2, 0, 1, 1);

        tableWidget = new QTableWidget(FormMenu);
        if (tableWidget->columnCount() < 7)
            tableWidget->setColumnCount(7);
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
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        gridLayout->addWidget(tableWidget, 3, 0, 1, 2);


        retranslateUi(FormMenu);

        QMetaObject::connectSlotsByName(FormMenu);
    } // setupUi

    void retranslateUi(QWidget *FormMenu)
    {
        FormMenu->setWindowTitle(QCoreApplication::translate("FormMenu", "Form", nullptr));
        pushButtonDelete->setText(QCoreApplication::translate("FormMenu", "delete ", nullptr));
        pushButtonAdd->setText(QCoreApplication::translate("FormMenu", "search", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("FormMenu", "Username", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("FormMenu", "Uid", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("FormMenu", "Commandtype", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("FormMenu", "pid", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("FormMenu", "Filepath", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("FormMenu", "Logtime", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("FormMenu", "Result", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormMenu: public Ui_FormMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMMENU_H
