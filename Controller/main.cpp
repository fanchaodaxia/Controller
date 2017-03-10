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
    // Following are a few examples of the command that we will use
    // Every command needs to be finished with GetResult, the next
    // command can only be sent when its return value "reault = 0"
    // Rotate the motor 1 so that the lienar actuator will move a distance of 10mm forward
    // Pre-condition: (1) Full step of the motor is 200 steps/revolution (1.8degree/step)
    //                (2) Microstepping level is x16
    // Note: This command is differnt from other command, a reply will be sent immediately after
    // the command initialization. We need to wait enough time for the motor to
    w.SendCmd(1,TMCL_MVP,MVP_REL,1,16000);
    result = w.GetResult(&add, &s, &v);

    // Rotate the motor 2 so that the linear actuaor will move a distance of 10mm backward
    w.SendCmd(1,TMCL_MVP,MVP_REL,1,-16000);
    result = w.GetResult(&add, &s, &v);

    // There are 6 motors in TMCM-6110 and numbered from 0 to 5
    w.SendCmd(1,TMCL_ROR,0,1,1000); //motor 1 turn right
    result = w.GetResult(&add, &s, &v);
    w.SendCmd(1,TMCL_ROR,0,2,1000); //motor 2 turn right
    result = w.GetResult(&add, &s, &v);

    // Turn on the LED light which is connected to OUT3 port of the controller
    w.SendCmd(1,TMCL_SIO,3,2,1);
    result = w.GetResult(&add, &s, &v);

    // Turn off the LED light
    w.SendCmd(1,TMCL_SIO,3,2,0);
    result = w.GetResult(&add, &s, &v);

    // Stop Motor 5
    w.SendCmd(1,TMCL_MST,0,5,0);
    result = w.GetResult(&add, &s, &v);

    // Stop Motor 1
    w.SendCmd(1,TMCL_MST,0,1,0);
    result = w.GetResult(&add, &s, &v);

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

