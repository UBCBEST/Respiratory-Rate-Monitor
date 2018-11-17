#include <tchar.h>
#include "SerialPort.h"
#include <engine.h>
#include <matrix.h>
#include <iostream>
#include <string>
#include <cmath>

#pragma comment (lib, "libmat.lib")
#pragma comment (lib, "libmx.lib")
#pragma comment (lib, "libmex.lib")
#pragma comment (lib, "libeng.lib")
#pragma comment (lib, "libut.lib")

// application reads from the specified serial port and reports the collected data
int main(int argc, _TCHAR* argv[]) {
	//Serial Arduino("\\\\.\\COM3");    // Adjust to match name of COM port 

	//if (Arduino.IsConnected())
	//	printf("We're connected \n");

	//printf("Initializing \n");		// TEMP: 
	////Sleep(2000);				// Wait for two seconds (arbitrary) to throw away the first few malformed bits (not entirely sure if this initial data will get ignored or placed in a FIFO)

	//char incomingData[256] = "";			// don't forget to pre-allocate memory
	//										//printf("%s\n",incomingData);
	//int dataLength = 255;
	//int readResult = 0;

	//while (Arduino.IsConnected())
	//{
	//	readResult = Arduino.ReadData(incomingData, dataLength);
	//	printf("Bytes read: (0 means no data available) %i\n", readResult);
	//	// Arduino data is received as a string, will also need to separate and pack
	//	// into individual variables (or a class/struct for each measurement
	//	if(readResult)
	//		Arduino.ParseRead(incomingData, readResult);

	//	Sleep(TIME_BETWEEN_SERIAL_WRITES);
	//}
	//return 0;

	Engine *m_pEngine;
	if (!(m_pEngine = engOpen("")))
	{
		fprintf(stderr, "\nCan't start MATLAB engine\n");
		return EXIT_FAILURE;
	}

	//Now call the MATLAB script through MATLAB Engine

	engEvalString(m_pEngine, "script");
	cout << "Hit return to continue\n\n";
	fgetc(stdin);

	//Close the MATLAB Engine

	engClose(m_pEngine);
	return EXIT_SUCCESS;
}