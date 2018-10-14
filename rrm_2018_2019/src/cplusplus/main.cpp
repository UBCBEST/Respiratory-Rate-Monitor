#include <tchar.h>
#include "SerialPort.h"	
#include <string.h>

// application reads from the specified serial port and reports the collected data
int main(int argc, _TCHAR* argv[]) {
	Serial Arduino("\\\\.\\COM5");    // Adjust to match name of COM port 

	if (Arduino.IsConnected())
		printf("We're connected \n");

	printf("Initializing \n");		// TEMP: 
	//Sleep(2000);				// Wait for two seconds (arbitrary) to throw away the first few malformed bits (not entirely sure if this initial data will get ignored or placed in a FIFO)

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
		if(readResult)
			Arduino.ParseRead(incomingData, readResult);

		Sleep(TIME_BETWEEN_SERIAL_WRITES);
	}
	return 0;
}