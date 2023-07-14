#ifndef FORMMENU_H
#define FORMMENU_H

#include <QWidget>
#include "task.h"
#include "formaddrule.h"
#include <QTimer>

namespace Ui {
class FormMenu;
}

class FormMenu : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenu(QWidget *parent = 0);
    ~FormMenu();
    void update(task t);
    void Init();

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

    QTimer *m_myTimer = new QTimer();

    QString User;


    FormAddRule *formChildAddRule;

    QString pwdOfUserTxt;

    Ui::FormMenu *ui;

    QMap<int, task> *m;


};

static int callback_search(void *data, int argc, char **argv, char **azColName);
static int count_callback(void* data, int n_cols, char** values, char** columns);

#endif // FORMMENU_H
