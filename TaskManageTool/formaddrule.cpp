#include "formaddrule.h"
#include "ui_formaddrule.h"
#include <QMessageBox>

FormAddRule::FormAddRule(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormAddRule)
{
    ui->setupUi(this);
    ui->dateTimeEditRemind->setDisplayFormat("yyyy-MM-dd-hh-mm-ss");
    ui->dateTimeEditRemind->setCalendarPopup(true);
}

FormAddRule::~FormAddRule()
{
    delete ui;
}

void FormAddRule::RecvAddButtonPressed(QMap<int, task> *m)
{
    mapChildPointer = m;
    ui->lineEditPath->clear();
    ui->dateTimeEditRemind->setDateTime(QDateTime::currentDateTime());
    ui->comboBoxRe->setCurrentIndex(0);
    ui->lineEditUsername->clear();
    ui->comboBoxCmd->setCurrentIndex(0);

}

void FormAddRule::on_pushButtonOK_clicked()
{

    QString Username =ui->lineEditUsername->text().trimmed();
    if(Username.isEmpty())
    {
        QMessageBox::information(this, tr("check"), tr("Username cannot be empty."));
        return;
    }
    if(Username.contains(" "))
    {
        QMessageBox::information(this, tr("check"), tr("Name cannot contain whitespace."));
        return;
    }
    QString file_path =ui->lineEditPath->text().trimmed();

    QString Logtime = ui->dateTimeEditRemind->dateTime()
            .toString("yyyy-MM-dd-hh-mm-ss");
    QString cmdType = ui->comboBoxCmd->currentText();
    QString Result = ui->comboBoxRe->currentText();

    t.Username = Username;
    t.File_path=file_path;
    if(strcmp(t.LogTime.toUtf8().data(),"All")!=0) t.LogTime=Logtime;
    t.Result=Result;
    t.Commondname=cmdType;

     emit sendSearch(t);

}


void FormAddRule::on_pushButton_clicked()
{
    t.LogTime="All";
}

