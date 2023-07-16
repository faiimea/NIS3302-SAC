#ifndef FORMCREATENEWUSER_H
#define FORMCREATENEWUSER_H

#include <QWidget>
#include <QCryptographicHash>

namespace Ui {
class FormCreateNewUser;
}

class FormCreateNewUser : public QWidget
{
    Q_OBJECT

public:
    explicit FormCreateNewUser(QWidget *parent = 0);
    ~FormCreateNewUser();

signals:
    void SendNewAccountCreated(QString strUsername,QString strPassword);

private slots:
    void on_pushButtonOK_clicked();
    void RecvCreateNewAccount(QMap<QString, QByteArray> *m);

private:
    Ui::FormCreateNewUser *ui;
    QMap<QString, QByteArray> *mapUserAndHash;
    void Init();
};

#endif // FORMCREATENEWUSER_H
