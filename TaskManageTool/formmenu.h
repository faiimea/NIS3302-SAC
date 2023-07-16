#ifndef FORMMENU_H
#define FORMMENU_H

#include <QWidget>
#include "task.h"
#include "formaddrule.h"
#include <QTimer>
#include <QDateTime>
namespace Ui {
class FormMenu;
}

class FormMenu : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenu(QWidget *parent = 0);
    ~FormMenu();
    void update();
    void Init();
    friend void copy(char a[],FormMenu *b);
    QTimer *m_myTimer = new QTimer();

    QString User;
    void show_all_log(char username[]);

    FormAddRule *formChildAddRule;

    QString pwdOfUserTxt;

    Ui::FormMenu *ui;

    QMap<int, task> *m;
private slots:

    void TimerResponse();

    void on_pushButtonAdd_clicked();

    //void on_pushButtonLoad_clicked();

    void on_pushButtonDelete_clicked();

    //void on_pushButtonDisplay_clicked();

    void RecvSignInButtonPressed(QString strUser);

    void RecvSearch(task t);

signals:

    void sendAddButtonPressed(QMap<int, task> *m);

private:
    char *sql;
};

void copy(char **a,FormMenu *b);

static int back(void *data, int argc, char **argv, char **azColName);

#endif // FORMMENU_H
