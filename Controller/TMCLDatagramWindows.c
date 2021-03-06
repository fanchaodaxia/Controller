//Example for communicating with TMCM modules over RS232 under Windows.


#include <windows.h>
#include "TMCLDatagramWindows.h"

//Open serial interface
//Usage: ComHandle=OpenRS232("COM1", CBR_9600)
HANDLE OpenRS232(const char* ComName, DWORD BaudRate)
{
	HANDLE ComHandle;
	DCB CommDCB;
	COMMTIMEOUTS CommTimeouts;

	ComHandle=CreateFile(ComName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(GetLastError()!=ERROR_SUCCESS) return INVALID_HANDLE_VALUE;
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
		SetupComm(ComHandle, 100, 100);  //Empfangspuffer, Sendepuffer (auch andere Werte m�glich)

		//Set up timeout values (very important, as otherwise the program will be very slow)
    GetCommTimeouts(ComHandle, &CommTimeouts);

		CommTimeouts.ReadIntervalTimeout=MAXDWORD;
		CommTimeouts.ReadTotalTimeoutMultiplier=0;
		CommTimeouts.ReadTotalTimeoutConstant=0;

		SetCommTimeouts(ComHandle, &CommTimeouts);

		return ComHandle;
  }
}


//Close the serial port
//Usage: CloseRS232(ComHandle);
void CloseRS232(HANDLE Handle)
{
	CloseHandle(Handle);
}


//Send a binary TMCL command
//e.g.  SendCmd(ComHandle, 1, TMCL_MVP, MVP_ABS, 1, 50000);   will be MVP ABS, 1, 50000 for a module with address 1
//Parameters: Handle: Handle of the serial port (returned by OpenRS232).
//            Address: address of the module (factory default is 1).
//            Command: the TMCL command (see the constants at the begiining of this file)
//            Type:    the "Type" parameter of the TMCL command (set to 0 if unused)
//            Motor:   the motor number (set to 0 if unused)
//            Value:   the "Value" parameter (depending on the command, set to 0 if unused)
//Senden eines TMCL-Befehls in bin�rer Form.
//z.B.  SendCmd(ComHandle, 1, TMCL_MVP, MVP_ABS, 1, 50000);  entsricht MVP ABS, 1, 50000 f�r Modul mit Adresse 1.
//Parameter: Handle: Handle der RS232-Schnittstelle (wird von OpenRS232 zur�ckgegeben).
//           Address: Adresse des Moduls (bei nur einem Modul meistens 1).
//           Command: Das TMCL-Kommando (siehe die in der TMCLDatagramWindows.h-Datei definierten Konstanten).
//           Type:    Der Typ-Parameter des TMCL-Befehls.
//           Motor:   Die Nummer des Motors.
//           Value:   Wert, (z.B. Position oder Geschwindigkeit, abh�ngig vom Befehl).
void SendCmd(HANDLE Handle, UCHAR Address, UCHAR Command, UCHAR Type, UCHAR Motor, INT Value)
{
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

	//Senden
	WriteFile(Handle, TxBuffer, 9, &BytesWritten, NULL);
}


//Read the result that is returned by the module
//Parameters: Handle: handle of the serial port, as returned by OpenRS232
//            Address: pointer to variable to hold the reply address returned by the module
//            Status: pointer to variable to hold the status returned by the module (100 means okay)
//            Value: pointer to variable to hold the value returned by the module
//Return value: TMCL_RESULT_OK: result has been read without errors
//              TMCL_RESULT_NOT_READY: not enough bytes read so far (try again)
//              TMCL_RESULT_CHECKSUM_ERROR: checksum of reply packet wrong
//Einlesen des vom Modul zur�ckgebenen Datenpaketes.
//Parameter: Handle: Handle der seriellen Schnittstelle (wird von OpenRS232 zruc�kgegeben)
//           Address: Die Adresse des Moduls wird hier abgelegt
//           Status: Der Status des Moduls wird hier abgelegt (z.B. 100 f�r alles in Ordnung).
//           Value: Der R�ckgabewert eines Abfragebefehls wird hier abgelegt.
//R�ckgabewert: TMCL_RESULT_OK: Daten wurden erfolgreich eingelesen
//              TMCL_RESULT_NOT_READY: Es konnten keine Daten eingelesen werden.
//              TMCL_RESULT_CHECKSUM_ERROR: Pr�fumme falsch.
UCHAR GetResult(HANDLE Handle, UCHAR *Address, UCHAR *Status, int *Value)
{
	UCHAR RxBuffer[9], Checksum;
	DWORD Errors, BytesRead;
	COMSTAT ComStat;
	int i;

	ClearCommError(Handle, &Errors, &ComStat);
  if(ComStat.cbInQue>8)
	{
		//Empfangen
		ReadFile(Handle, RxBuffer, 9, &BytesRead, NULL);

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

