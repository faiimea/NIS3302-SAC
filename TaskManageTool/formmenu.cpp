#include "formmenu.h"
#include "ui_formmenu.h"
#include <qdir.h>
#include <stdio.h>
#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <thread>
#define TM_FMT "%Y-%m-%d %H:%M:%S"
#define NETLINK_TEST 29
#define MAX_PAYLOAD 1024  /* maximum payload size*/



FormMenu::FormMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenu)
{
    ui->setupUi(this);

}

FormMenu::~FormMenu()
{
    delete m;
    m = NULL;
    delete formChildAddRule;
    formChildAddRule = NULL;
    delete ui;
    m_myTimer->stop();
}

int max(int a, int b)
{
    return a > b ? a : b;
}

void FormMenu::Init()
{
    char temp[30];
    strcpy(temp,User.toStdString().data());
    create_table(temp);

    task t;
    int uid,pid;

    char Username[100], commandname[100],  logtime[100],file_path[100],result[10];


    formChildAddRule = NULL;
    formChildAddRule = new FormAddRule(NULL);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(36);
    ui->tableWidget->setIconSize(QSize(32, 32));
    QHeaderView* headerView = ui->tableWidget->verticalHeader();
    headerView->setHidden(true);

    //update();

    connect(this, SIGNAL(sendAddButtonPressed(QMap<int,task>*)),
            formChildAddRule, SLOT(RecvAddButtonPressed(QMap<int,task>*)));
    connect(formChildAddRule, SIGNAL(sendSearch(task)),
            this, SLOT(RecvSearch(task)));
    connect(m_myTimer, SIGNAL(timeout()), this, SLOT(TimerResponse()));

    //m_myTimer->start(1000);
    std::thread thread_(solve);
    thread_.detach();

}

void FormMenu::update(task t )
{
    ui->tableWidget->setSortingEnabled(false);

    /*while(ui->tableWidget->rowCount() > 0)
    {
        int nRow = ui->tableWidget->rowCount();
        ui->tableWidget->removeRow(nRow - 1);
    }*/
        int nRow = ui->tableWidget->rowCount();


        QString wStr;

        wStr = "";

        //qDebug() << ID;

        ui->tableWidget->insertRow(nRow);
        wStr += t.Username + " ";
        QTableWidgetItem *itemUserame = new QTableWidgetItem(t.Username);
        ui->tableWidget->setItem(nRow, 0, itemUserame);


        wStr += QString(t.uid) + " ";
        QTableWidgetItem *itemUid = new QTableWidgetItem(QString(t.uid));
        ui->tableWidget->setItem(nRow, 0, itemUid);


        wStr += t.Commondname + " ";
        QTableWidgetItem *itemCommandname = new QTableWidgetItem(t.Commondname);
        ui->tableWidget->setItem(nRow, 0, itemCommandname);


        wStr += QString(t.pid) + " ";
        QTableWidgetItem *itemPid= new QTableWidgetItem(QString(t.pid));
        ui->tableWidget->setItem(nRow, 0, itemPid);



        wStr += t.LogTime + " ";
        QTableWidgetItem *itemLogtime = new QTableWidgetItem(t.LogTime);
        ui->tableWidget->setItem(nRow, 0, itemLogtime);



        wStr += t.File_path + " ";
        QTableWidgetItem *itemFilepath = new QTableWidgetItem(t.File_path);
        ui->tableWidget->setItem(nRow, 0, itemFilepath);



        wStr += t.Result + "\n";
        QTableWidgetItem *itemResult = new QTableWidgetItem(t.Result);
        ui->tableWidget->setItem(nRow, 0, itemResult);


        //qDebug() << wStr;

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);
}

void FormMenu::RecvSignInButtonPressed(QString strUser)
{
    User = strUser;
    Init();
}

void FormMenu::TimerResponse()
{

}

void FormMenu::RecvSearch(task t)
{
    formChildAddRule->hide();
}

void FormMenu::on_pushButtonAdd_clicked()
{
    emit sendAddButtonPressed(m);

    formChildAddRule->show();
    if(formChildAddRule->isMinimized())
    {
        formChildAddRule->showNormal();
    }
    formChildAddRule->raise();
}



void FormMenu::on_pushButtonDelete_clicked()
{
    QTableWidgetItem *curItem = ui->tableWidget->currentItem();
    if(curItem != NULL)
    {
        if(curItem->isSelected())
        {
            int id = ui->tableWidget->item(curItem->row(), 0)->text().toInt();
            ui->tableWidget->removeRow(curItem->row());
            //delete_log
        }
    }
}
