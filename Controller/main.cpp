#include "controller.h"
#include <QApplication>
#include <tmcl.h>
#include <QDebug>
#include <QChar>
//#include <QTest>
#include <QThread>

class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};

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
    Sleeper::sleep(1);
    //w.SendCmd(1,TMCL_ROR,0,2,1000);

    result = w.GetResult(add,status,value);
    qDebug() << *add;
    qDebug() << *status;
    qDebug() << *value;
    qDebug() << "The result" << result;
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

