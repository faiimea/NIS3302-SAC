#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QCryptographicHash>
#include <QMap>
#include "formchangepassword.h"
#include "formcreatenewuser.h"
#include "formmenu.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    QMap<QString, QByteArray> mapUserAndHash;
    Ui::MainWidget *ui;

    FormChangePassword *formChildChangePassword;

    FormCreateNewUser *formChildCreatorNewUser;
    FormMenu *formChildMenu;

signals:

    void SendOldUserPassword(QMap<QString, QByteArray> *m);

    void SendCreateNewAccount(QMap<QString, QByteArray> *m);

    void SendSignInButtonPressed(QString strUser);

private slots:
    void on_pushButtonSignIn_clicked();

    void on_pushButtonSignUp_clicked();

    void on_pushButtonChange_clicked();

    void RecvNewUserPassword(QString strUsername,QString strPassword,QString newPassword);

    void RecvNewAccountCreated(QString strUsername,QString strPassword);



private:




    void Init();

    void readAccout();

    void writeAccout();
};



#endif // MAINWIDGET_H
