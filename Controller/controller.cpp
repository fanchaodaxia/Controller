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
    ui->label_status->setText("controller not connected");

// open the serial port
        DCB CommDCB;
        COMMTIMEOUTS CommTimeouts;
        LPCWSTR ComName = L"COM4";
        DWORD BaudRate = CBR_9600;
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
            SetupComm(ComHandle, 100, 100);  //Empfangspuffer, Sendepuffer (auch andere Werte möglich)

            //Set up timeout values (very important, as otherwise the program will be very slow)
        GetCommTimeouts(ComHandle, &CommTimeouts);

            CommTimeouts.ReadIntervalTimeout=MAXDWORD;
            CommTimeouts.ReadTotalTimeoutMultiplier=0;
            CommTimeouts.ReadTotalTimeoutConstant=0;
            SetCommTimeouts(ComHandle, &CommTimeouts);
            ui->label_status->setText("COM5 connected");
      }
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

    //Send data out
    WriteFile(ComHandle, TxBuffer, 9, &BytesWritten, NULL);

}


//Read the result that is returned by the module
//Parameters: Handle: handle of the serial port, as returned by OpenRS232
//            Address: pointer to variable to hold the reply address returned by the module
//            Status: pointer to variable to hold the status returned by the module (100 means okay)
//            Value: pointer to variable to hold the value returned by the module
//Return value: TMCL_RESULT_OK: result has been read without errors
//              TMCL_RESULT_NOT_READY: not enough bytes read so far (try again)
//              TMCL_RESULT_CHECKSUM_ERROR: checksum of reply packet wrong

UCHAR Controller::GetResult(UCHAR *Address, UCHAR *Status, INT *Value)
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
