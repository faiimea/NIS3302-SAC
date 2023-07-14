#include "formchangepassword.h"
#include "ui_formchangepassword.h"
#include "api.h"
#include <QMessageBox>

FormChangePassword::FormChangePassword(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormChangePassword)
{
    ui->setupUi(this);

    Init();
}

FormChangePassword::~FormChangePassword()
{
    delete ui;
}

void FormChangePassword::Init()
{
    setWindowTitle( tr("change password"));

    ui->lineEditOldPassword->setEchoMode(QLineEdit::Password);
    ui->lineEditNewPassword->setEchoMode(QLineEdit::Password);
    ui->lineEditConfirm->setEchoMode(QLineEdit::Password);

}

void FormChangePassword::RecvOldUserPassword(QMap<QString, QByteArray>* m)
{
    mapUserAndHash = m;

    ui->lineEditUser->clear();
    ui->lineEditOldPassword->clear();
    ui->lineEditNewPassword->clear();
    ui->lineEditConfirm->clear();
}

void FormChangePassword::on_pushButtonOK_clicked()
{
    QString strUser = ui->lineEditUser->text().trimmed();
    QString strOldPassword = ui->lineEditOldPassword->text().trimmed();
    QString strNewPassword = ui->lineEditNewPassword->text().trimmed();
    QString strConfirm = ui->lineEditConfirm->text().trimmed();

    if(strUser.isEmpty() || strOldPassword.isEmpty() || strNewPassword.isEmpty()
            || strConfirm.isEmpty())
    {
        QMessageBox::information(this, tr("check"), tr("User or password cannot be empty."));
        return;
    }

    if(strNewPassword != strConfirm)
    {
        QMessageBox::information(this, tr("check"), tr("Two new passwords are not the same."));
        return;
    }




    emit SendNewUserPassword(strUser,strOldPassword,strNewPassword);
}
