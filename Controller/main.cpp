#include "controller.h"
#include <QApplication>
#include <tmcl.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller w;
    w.show();
    w.SendCmd(1,TMCL_MVP,MVP_REL,1,2000);
    return a.exec();
}
