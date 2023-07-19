#ifndef FORMMENU_H
#define FORMMENU_H

#include <QWidget>
#include "task.h"
#include "formaddrule.h"
#include <QTimer>
#include <QDateTime>
#include <QTableWidget>
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
    void show_all_log();

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

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:

    void sendAddButtonPressed(QMap<int, task> *m);

private:
    char *sql;
    std::thread thread1;
};

void copy(char **a,FormMenu *b);

static int back(void *data, int argc, char **argv, char **azColName);

void sortTableByColumn(QTableWidget *tableWidget, int column);

#endif // FORMMENU_H
