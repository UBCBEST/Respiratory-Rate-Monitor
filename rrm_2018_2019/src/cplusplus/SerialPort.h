// Base serial read header and source files retrieved from https://playground.arduino.cc/Interfacing/CPPWindows
// Requires the Microsoft .NET Framework to make use of System.IO
#ifndef SERIALPORT_H_INCLUDED
#define SERIALPORT_H_INCLUDED

#define ARDUINO_WAIT_TIME 2000
#define TIME_BETWEEN_SERIAL_WRITES 50	// in ms

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "AccelData.h"

class Serial
{
private:
	//Serial comm handler
	HANDLE hSerial;
	//Connection status
	bool connected;
	//Get various information about the connection
	COMSTAT status;
	//Keep track of last error
	DWORD errors;

	// Internal character buffer for ParseRead
	char ParseBuffer[256] = "";	// Initialised as empty

	Accel_Data AccelData;	// Acceleration data object
public:
	//Initialize Serial communication with the given COM port
	Serial(const char *portName);
	//Close the connection
	~Serial();
	//Read data in a buffer, if nbChar is greater than the
	//maximum number of bytes available, it will return only the
	//bytes available. The function return -1 when nothing could
	//be read, the number of bytes actually read.
	int ReadData(char *buffer, unsigned int nbChar);
	void ParseRead(char *inputBuffer, int nbChar);
	//Writes data from a buffer through the Serial connection
	//return true on success.
	bool WriteData(const char *buffer, unsigned int nbChar);
	// Status flag for connection to the serial device 
	bool IsConnected();
};

#endif // SERIALPORT_H_INCLUDED