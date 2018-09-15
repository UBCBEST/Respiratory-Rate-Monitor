#include <stdio.h>
#include <tchar.h>
#include "SerialPort.h"	
#include <string.h>

using namespace std;

// application reads from the specified serial port and reports the collected data
int main(int argc, _TCHAR* argv[]) {
	Serial Arduino("\\\\.\\COM5");    // Adjust to match name of COM port 

	if (Arduino.IsConnected())
		printf("We're connected");

	char incomingData[256] = "";			// don't forget to pre-allocate memory
											//printf("%s\n",incomingData);
	int dataLength = 255;
	int readResult = 0;

	while (Arduino.IsConnected())
	{
		readResult = Arduino.ReadData(incomingData, dataLength);
		printf("Bytes read: (0 means no data available) %i\n", readResult);
		// Arduino data is received as a string, will also need to separate and pack
		// into individual variables (or a class/struct for each measurement
		incomingData[readResult] = 0;

		printf("%s", incomingData);
		Sleep(500);
	}
	return 0;
}