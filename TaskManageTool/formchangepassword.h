#ifndef FORMCHANGEPASSWORD_H
#define FORMCHANGEPASSWORD_H

#include <QWidget>
#include <QCryptographicHash>

namespace Ui {
class FormChangePassword;
}

class FormChangePassword : public QWidget
{
    Q_OBJECT

public:
    explicit FormChangePassword(QWidget *parent = 0);
    ~FormChangePassword();


signals:

    void SendNewUserPassword(QString strUsername,QString strPassword,QString newPassword);

private slots:
    void on_pushButtonOK_clicked();

    void RecvOldUserPassword(QMap<QString, QByteArray>* m);

private:
    Ui::FormChangePassword *ui;

    void Init();

    QMap<QString, QByteArray> *mapUserAndHash;

    QString strUser;
    QByteArray m_baOldHash;
};

#endif // FORMCHANGEPASSWORD_H
