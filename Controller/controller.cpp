#include "controller.h"
#include "ui_controller.h"
#include "qdebug.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtWidgets>
#include <windows.h>
#include <tmcl.h>
#include <stdlib.h>
#include <iostream>



Controller::Controller(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Controller)
{
    ui->setupUi(this);
    ui->label_status->setText("controller not connected");

// open the serial port
        DCB CommDCB;
        COMMTIMEOUTS CommTimeouts;
        LPCWSTR ComName = L"COM7"; // Com port used for the controller
        DWORD BaudRate = CBR_9600;
        bool IsControllerConnected = false;
        UCHAR add, s;
        INT v;
        ComHandle=CreateFile(ComName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if(GetLastError()!=ERROR_SUCCESS)
        {
            QMessageBox::warning(this,"port error","connection not success");
        }
        else
        {
            GetCommState(ComHandle, &CommDCB);
            CommDCB.BaudRate=BaudRate;
            CommDCB.Parity=NOPARITY;
            CommDCB.StopBits=ONESTOPBIT;
            CommDCB.ByteSize=8;
            CommDCB.fBinary=1;  //Binary Mode only
            CommDCB.fParity=0;
            CommDCB.fOutxCtsFlow=0;
            CommDCB.fOutxDsrFlow=0;
            CommDCB.fDtrControl=0;
            CommDCB.fDsrSensitivity=0;
            CommDCB.fTXContinueOnXoff=0;
            CommDCB.fOutX=0;
            CommDCB.fInX=0;
            CommDCB.fErrorChar=0;
            CommDCB.fNull=0;
            CommDCB.fRtsControl=RTS_CONTROL_TOGGLE;
            CommDCB.fAbortOnError=0;
            SetCommState(ComHandle, &CommDCB);

            //Set buffer size
            SetupComm(ComHandle, 100, 100);

            //Set up timeout values (very important, as otherwise the program will be very slow)
            GetCommTimeouts(ComHandle, &CommTimeouts); 
            CommTimeouts.ReadIntervalTimeout=MAXDWORD;
            CommTimeouts.ReadTotalTimeoutMultiplier=0;
            CommTimeouts.ReadTotalTimeoutConstant=0;
            SetCommTimeouts(ComHandle, &CommTimeouts);
            IsControllerConnected = true;
            ui->label_status->setText("Controller connected");

      }
        // If connection if connected succesful, initialize the controller
        // Assume only motor 0 and motor 1 will be used here
        if (IsControllerConnected)
        {
            SendCmd(1,TMCL_SAP,4,0,1400);//set motor 0 maximum velocity
            while(GetResult(&add, &s, &v)); // Wait untile the reault is OK
            SendCmd(1,TMCL_SAP,4,1,1400);//set motor 1 maximum velocity
            while(GetResult(&add, &s, &v));

            SendCmd(1,TMCL_SAP,5,0,1200);//set motor 0 maximum acceleration
            while(GetResult(&add, &s, &v));
            SendCmd(1,TMCL_SAP,5,1,1200);//set motor 1 maximum acceleration
            while(GetResult(&add, &s, &v));

            // This the most IMPORTANT parameter!!!
            SendCmd(1,TMCL_SAP,6,0,197);//set motor 0 maximum current to 0.85A
            while(GetResult(&add, &s, &v));
            SendCmd(1,TMCL_SAP,6,1,197);//set motor 1 maximum current to 0.85A
            while(GetResult(&add, &s, &v));

            SendCmd(1,TMCL_SAP,7,0,20);//set motor 0 standby current to 0.1A
            while(GetResult(&add, &s, &v));
            SendCmd(1,TMCL_SAP,7,1,20);//set motor 1 standby current to 0.1A
            while(GetResult(&add, &s, &v));

            SendCmd(1,TMCL_SAP,140,0,4);//set motor 0 microstepping resolution to 16 microsteps
            while(GetResult(&add, &s, &v));
            SendCmd(1,TMCL_SAP,140,1,4);//set motor 1 microstepping resolution to 16 microsteps
            while(GetResult(&add, &s, &v));
        }
    }




Controller::~Controller()
{
    CloseHandle(ComHandle);// close the serial port, disconnect controller

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

    //Send data out
    WriteFile(ComHandle, TxBuffer, 9, &BytesWritten, NULL);

}



UCHAR Controller::GetResult(UCHAR *Address, UCHAR *Status, INT *Value)
{
    UCHAR RxBuffer[9], Checksum;
    DWORD Errors, BytesRead;
    COMSTAT ComStat;
    int i;
    unsigned char CmdEcho = TMCL_RESULT_NOT_READY;

    do
    {
        ClearCommError(ComHandle, &Errors, &ComStat);
        if(ComStat.cbInQue>8)
        {
            //Empfangen
            ReadFile(ComHandle, RxBuffer, 9, &BytesRead, NULL);
            Checksum=0;
            for(i=0; i<8; i++)
                Checksum+=RxBuffer[i];
            if(Checksum!=RxBuffer[8])
              CmdEcho = TMCL_RESULT_CHECKSUM_ERROR;
            else
            {
              CmdEcho = TMCL_RESULT_OK;
              printf("Command echo OK! \n");
            }

            *Address = RxBuffer[0];
            *Status = RxBuffer[2];
            *Value = (RxBuffer[4] << 24) | (RxBuffer[5] << 16) | (RxBuffer[6] << 8) | RxBuffer[7];
        }
        else
          CmdEcho = TMCL_RESULT_NOT_READY;
    }
    while (CmdEcho != TMCL_RESULT_OK);

    return CmdEcho;

}

