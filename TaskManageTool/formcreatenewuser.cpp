#include "formcreatenewuser.h"
#include "ui_formcreatenewuser.h"
#include <QMessageBox>

FormCreateNewUser::FormCreateNewUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCreateNewUser)
{
    ui->setupUi(this);

    Init();
}

FormCreateNewUser::~FormCreateNewUser()
{
    delete ui;
}

void FormCreateNewUser::Init()
{
    setWindowTitle( tr("create new user"));

    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
    ui->lineEditConfirm->setEchoMode(QLineEdit::Password);
}

void FormCreateNewUser::RecvCreateNewAccount(QMap<QString, QByteArray> *m)
{
    mapUserAndHash = m;

    ui->lineEditUser->clear();
    ui->lineEditPassword->clear();
    ui->lineEditConfirm->clear();
}

void FormCreateNewUser::on_pushButtonOK_clicked()
{
    QString strNewUser = ui->lineEditUser->text().trimmed();
    QString strPassword = ui->lineEditPassword->text().trimmed();
    QString strConfirm = ui->lineEditConfirm->text().trimmed();

    if(strNewUser.contains(" "))
    {
        QMessageBox::information(this, tr("check"), tr("Please don't use whitespace in your user name."));
        return;
    }

    if(strNewUser.isEmpty() || strPassword.isEmpty() || strConfirm.isEmpty())
    {
        QMessageBox::information(this, tr("check"), tr("User or Password cannot be empty."));
        return;
    }

    if((*mapUserAndHash).contains(strNewUser))
    {
        QMessageBox::information(this, tr("check"), tr("User name already exists."));
        return;
    }

    if(strPassword != strConfirm)
    {
        QMessageBox::information(this, tr("check"), tr("Two passwords are not the same."));
        return;
    }



    emit SendNewAccountCreated(strNewUser,strPassword);
}
