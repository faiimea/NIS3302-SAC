#include "mainwidget.h"
#include <QApplication>
#include "api.h"
#include "formmenu.h"
#include "ui_formmenu.h"



int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    MainWidget w;
    w.show();
    return a.exec();
}


