#include "controller.h"
#include "ui_controller.h"
#include "qdebug.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtWidgets>
#include <windows.h>
#include <tmcl.h>
#include <stdlib.h>



Controller::Controller(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Controller)
{
    ui->setupUi(this);

    // initialization of the variables
    controller_status = "Not connected";
    controller_is_avalable = false;
    controller_port_name = "";
    controller = new QSerialPort;
    ui->label_status->setText(controller_status);// initial state of the communication

// open the serial port

    if (controller_is_avalable)
    {
        //open and configure the serialpoort
        controller->setPortName(controller_port_name);
        controller->open(QSerialPort::ReadWrite);
        controller->setBaudRate(QSerialPort::Baud9600);
        controller->setDataBits(QSerialPort::Data8);
        controller->setParity(QSerialPort::NoParity);
        controller->setStopBits(QSerialPort::OneStop);
        controller->setFlowControl(QSerialPort::NoFlowControl);
        controller_status = controller_port_name+"Controller Connected";
        ui->label_status->setText(controller_status);
    }
    else
    {
        // give error massage if not avalable
       QMessageBox::warning(this,"Port error","Connot find the controller!");
    }

    ui->label_status->setText(controller_port_name+"connected");

 }


Controller::~Controller()
{
    if(controller->isOpen()){
        controller->close();
    }
    delete ui;
}


//Send a binary TMCL command
//e.g.  SendCmd(ComHandle, 1, TMCL_MVP, MVP_ABS, 1, 50000);   will be MVP ABS, 1, 50000 for a module with address 1
//Parameters: Handle: Handle of the serial port (returned by OpenRS232).
//            Address: address of the module (factory default is 1).
//            Command: the TMCL command (see the constants at the begiining of this file)
//            Type:    the "Type" parameter of the TMCL command (set to 0 if unused)
//            Motor:   the motor number (set to 0 if unused)
//            Value:   the "Value" parameter (depending on the command, set to 0 if unused)
void Controller::SendCmd(UCHAR Address, UCHAR Command, UCHAR Type, UCHAR Motor, INT Value){
    UCHAR TxBuffer[9];
    DWORD BytesWritten;
    int i;

    TxBuffer[0]=Address;
    TxBuffer[1]=Command;
    TxBuffer[2]=Type;
    TxBuffer[3]=Motor;
    TxBuffer[4]=Value >> 24;
    TxBuffer[5]=Value >> 16;
    TxBuffer[6]=Value >> 8;
    TxBuffer[7]=Value & 0xff;
    TxBuffer[8]=0;
    for(i=0; i<8; i++)
        TxBuffer[8]+=TxBuffer[i];

    //send out data to controller

    //WriteFile(ComHandle, TxBuffer, 9, &BytesWritten, NULL); // old way
    controller->write()


}


//Read the result that is returned by the module
//Parameters: Handle: handle of the serial port, as returned by OpenRS232
//            Address: pointer to variable to hold the reply address returned by the module
//            Status: pointer to variable to hold the status returned by the module (100 means okay)
//            Value: pointer to variable to hold the value returned by the module
//Return value: TMCL_RESULT_OK: result has been read without errors
//              TMCL_RESULT_NOT_READY: not enough bytes read so far (try again)
//              TMCL_RESULT_CHECKSUM_ERROR: checksum of reply packet wrong

UCHAR Controller::GetResult(UCHAR *Address, UCHAR *Status, int *Value)
{
    UCHAR RxBuffer[9], Checksum;
    DWORD Errors, BytesRead;
    COMSTAT ComStat;
    int i;
    ClearCommError(ComHandle, &Errors, &ComStat);
  if(ComStat.cbInQue>8)
    {
        //Empfangen
        ReadFile(ComHandle, RxBuffer, 9, &BytesRead, NULL);

        Checksum=0;
        for(i=0; i<8; i++)
            Checksum+=RxBuffer[i];

        if(Checksum!=RxBuffer[8]) return TMCL_RESULT_CHECKSUM_ERROR;
        *Address=RxBuffer[0];
        *Status=RxBuffer[2];
        *Value=(RxBuffer[4] << 24) | (RxBuffer[5] << 16) | (RxBuffer[6] << 8) | RxBuffer[7];
    } else return TMCL_RESULT_NOT_READY;

    return TMCL_RESULT_OK;
}
