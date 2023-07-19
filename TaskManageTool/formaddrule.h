#ifndef FORMADDRULE_H
#define FORMADDRULE_H

#include <QWidget>
#include "task.h"
namespace Ui {
class FormAddRule;
}

class FormAddRule : public QWidget
{
    Q_OBJECT

public:
    explicit FormAddRule(QWidget *parent = 0);
    ~FormAddRule();

signals:
    void sendSearch(task t);

private slots:
    void on_pushButtonOK_clicked();

    void RecvAddButtonPressed(QMap<int, task> *m);

    void on_pushButton_clicked();

private:
    QMap<int, task> *mapChildPointer;
    Ui::FormAddRule *ui;
    task t;
};

#endif // FORMADDRULE_H
