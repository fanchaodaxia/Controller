#include "controller.h"
#include <QApplication>
#include <tmcl.h>
#include <QDebug>
#include <QChar>
//#include <QTest>
#include <QThread>
#include <iostream>
#include <stdlib.h>



int main(int argc, char *argv[])
{
    UCHAR add, s;
    UCHAR result;
    INT v;
    QApplication a(argc, argv);
    Controller w;
    w.show();
    //w.SendCmd(1,TMCL_MVP,MVP_REL,1,2000);
   //result = w.GetResult(&add, &s, &v);
    //qDebug() << "status from debug before" << s <<endl;

    w.SendCmd(1,TMCL_ROR,0,1,1000); //motor 1 turn right
    result = w.GetResult(&add, &s, &v);

    w.SendCmd(1,TMCL_ROR,0,2,1000); //motor 2 turn right
    result = w.GetResult(&add, &s, &v);

    //Sleep(5000);



    /*w.SendCmd(1,TMCL_MST,0,1,0);// motor 1 stop
    result = w.GetResult(&add, &s, &v);

    w.SendCmd(1,TMCL_MST,0,2,0);// motor 2 stop
    result = w.GetResult(&add, &s, &v);

    w.SendCmd(1,TMCL_GIO,1,0,0);// Read in digital input, port 1, bank 0, value don't care (0)
    result = w.GetResult(&add, &s, &v);
    printf("The input status is %u \n",v);*/






    return a.exec();
}

/*void QTest::qSleep(int ms)
{
    QTEST_ASSERT(ms > 0);

#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}*/

