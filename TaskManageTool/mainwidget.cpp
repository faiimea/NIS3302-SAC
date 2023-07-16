#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QMessageBox>
#include "api.h"
#include <stdio.h>
MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    Init();
}

MainWidget::~MainWidget()
{
    delete formChildChangePassword;
    formChildChangePassword = NULL;
    delete formChildCreatorNewUser;
    formChildCreatorNewUser = NULL;
    delete ui;
}

void MainWidget::Init()
{
    //setMapUserAndHash();

    setWindowTitle( tr("TaskManageTool"));

    ui->lineEditPassword->setEchoMode(QLineEdit::Password);

    formChildChangePassword = NULL;
    formChildCreatorNewUser = NULL;
    formChildMenu = NULL;

    formChildChangePassword = new FormChangePassword(NULL);
    formChildCreatorNewUser = new FormCreateNewUser(NULL);
    formChildMenu = new FormMenu(NULL);

    connect(this, SIGNAL(SendOldUserPassword(QMap<QString,QByteArray>*)),
            formChildChangePassword, SLOT(RecvOldUserPassword(QMap<QString,QByteArray>*)));

    connect(formChildChangePassword, SIGNAL(SendNewUserPassword(QString ,QString,QString)),
            this, SLOT(RecvNewUserPassword(QString ,QString,QString)));

    connect(this, SIGNAL(SendCreateNewAccount(QMap<QString,QByteArray>*)),
            formChildCreatorNewUser, SLOT(RecvCreateNewAccount(QMap<QString,QByteArray>*)));

    connect(formChildCreatorNewUser, SIGNAL(SendNewAccountCreated(QString ,QString)),
            this, SLOT(RecvNewAccountCreated(QString ,QString)));

    connect(this, SIGNAL(SendSignInButtonPressed(QString)),
            formChildMenu, SLOT(RecvSignInButtonPressed(QString)));

    _init_database();

    create_users();

}

void MainWidget::readAccout()
{
    char s1[100], s2[100];
    FILE *fp = fopen("account.txt", "r");
    if(fp == NULL) return;
    while(fscanf(fp, "%s %s", s1, s2) != EOF)
    {
        QString str1 = QString(s1);
        QByteArray str2 = QByteArray(s2);
        mapUserAndHash.insert(str1, str2);
    }
    fclose(fp);
}

void MainWidget::writeAccout()
{
    FILE *fp = fopen("account.txt", "w+");
    QMap<QString, QByteArray>::iterator it;
    for(it = mapUserAndHash.begin(); it != mapUserAndHash.end(); it++)
    {
        fputs(it.key().toStdString().c_str(), fp);
        fputs(" ", fp);
        fputs(it.value().data(), fp);
        fputs("\n", fp);
    }
    fclose(fp);
}

void MainWidget::RecvNewUserPassword(QString strUsername,QString strPassword,QString newPassword)
{
    formChildChangePassword->hide();

    change_password(strUsername.toUtf8().data(),strPassword.toUtf8().data(),newPassword.toUtf8().data());
    //writeAccout();
}

void MainWidget::RecvNewAccountCreated(QString strUsername,QString strPassword)
{
    formChildCreatorNewUser->hide();
    insert_users(strUsername.toUtf8().data(),strPassword.toUtf8().data());

    //writeAccout();
}

void MainWidget::on_pushButtonSignIn_clicked()
{
    QString strUser = ui->lineEditUser->text().trimmed();
    QString strPassword = ui->lineEditPassword->text().trimmed();

    if(strUser.isEmpty() || strPassword.isEmpty())
    {
        QMessageBox::information(this, tr("check"), tr("User or password cannot be empty."));
        return;
    }

    /*if(!mapUserAndHash.contains(strUser))
    {
        QMessageBox::information(this, tr("check"), tr("User not exists."));
        return;
    }

    QByteArray baHash = QCryptographicHash::hash(strPassword.toUtf8(),
                                                 QCryptographicHash::Sha256);
    baHash = baHash.toHex();

    if(baHash != mapUserAndHash[strUser])
    {
        QMessageBox::information(this, tr("check"), tr("Password is wrong."));
        return;
    }*/


    int rc= search_users(strUser.toUtf8().data(),strPassword.toUtf8().data());
    if(rc){
        emit SendSignInButtonPressed(strUser);
        formChildMenu->show();
        if(formChildMenu->isMinimized())
        {
            formChildMenu->showNormal();
        }
        formChildMenu->raise();

        this->close();
    }
    else {
        std::string output = "notify-send --urgency=critical \"Username or Password is wrong.\"";
        system(output.c_str());
    }
}

void MainWidget::on_pushButtonSignUp_clicked()
{
    emit SendCreateNewAccount(&mapUserAndHash);

    formChildCreatorNewUser->show();
    if(formChildCreatorNewUser->isMinimized())
    {
        formChildCreatorNewUser->showNormal();
    }
    formChildCreatorNewUser->raise();
}

void MainWidget::on_pushButtonChange_clicked()
{
    emit SendOldUserPassword(&mapUserAndHash);

    formChildChangePassword->show();
    if(formChildChangePassword->isMinimized())
    {
        formChildChangePassword->showNormal();
    }
    formChildChangePassword->raise();
}


