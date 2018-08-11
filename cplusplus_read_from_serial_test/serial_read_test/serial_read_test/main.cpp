//Basic framework to read serial data from an Arduino.

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SerialPort.h"

using namespace std;

/*Portname must contain these backslashes, replace with the appropriate portname */
const char *port_name = "\\\\.\\COM20";

//String for incoming data
char incomingData[MAX_DATA_LENGTH];

int main()
{
	SerialPort arduino(port_name);
	if (arduino.isConnected()) cout << "Connection Established" << endl;
	else cout << "ERROR, check port name";

	while (arduino.isConnected()) {
		//Check if data has been read or not
		int read_result = arduino.readSerialPort(incomingData, MAX_DATA_LENGTH);
		//prints out data
		puts(incomingData);
		//wait a bit
		Sleep(10);
	}
}