#include "controller.h"
#include <QApplication>
#include <tmcl.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    UCHAR *add, *status;
    UCHAR result;
    int *value;
    QApplication a(argc, argv);
    Controller w;
    w.show();
    //w.SendCmd(1,TMCL_MVP,MVP_REL,1,2000);
    w.SendCmd(1,TMCL_ROR,0,1,1000);
    result = w.GetResult(add,status,value);
    qDebug() << *add;
    qDebug() << *status;
    qDebug() << *value;
    qDebug() << "The result" << result;
    return a.exec();
}


