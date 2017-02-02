#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QMainWindow>
#include <QSerialPort>
#include <Windows.h>

namespace Ui {
class Controller;
}

class Controller : public QMainWindow
{
    Q_OBJECT

public:
    explicit Controller(QWidget *parent = 0);
    ~Controller();
    void SendCmd(UCHAR Address, UCHAR Command, UCHAR Type, UCHAR Motor, INT Value);
    UCHAR GetResult(UCHAR *Address, UCHAR *Status, int *Value);
private:
    Ui::Controller *ui;
    QSerialPort *controller;
    static const  quint16 controller_vendorID = 9025;
    QString controller_port_name;
    bool controller_is_avalable;
    QString controller_status;
    HANDLE controller_handle;
};

#endif // CONTROLLER_H
