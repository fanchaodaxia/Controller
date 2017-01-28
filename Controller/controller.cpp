#include "controller.h"
#include "ui_controller.h"
#include "qdebug.h"
#include <QSerialPort>
#include <QSerialPortInfo>


Controller::Controller(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Controller)
{
    ui->setupUi(this);
    controller = new QSerialPort;

    qDebug() << "number of available ports:" << QSerialPortInfo::availablePorts().length();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
           qDebug() << "Name : " << info.portName();
           qDebug() << "Description : " << info.description();
           qDebug() << "Manufacturer: " << info.manufacturer();


       }
}

Controller::~Controller()
{
    delete ui;
}
