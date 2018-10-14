// Class container for the accelerometer data. Using a singular class to store the acceleration data from both accelerometers 
// due to how the data is currently sent by the serial device and due to how its parsed
#ifndef ACCEL_DATA_H_INCLUDED
#define ACCEL_DATA_H_INCLUDED

#define MAX_DATA_LENGTH 255
#define NUM_MEASUREMENTS_PER_SERIAL_LINE 12

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>


using namespace std;

class Accel_Data {
	public:
		Accel_Data();	// Default construtor
		//~Accel_Data();	// Default destructor
		bool Update_Data(char * DataFromSerial, int DataLength);	// Parses a parsed serial read string into our data array
		void set_print_val_flag(bool flag_val);
	private:
		double accelData[NUM_MEASUREMENTS_PER_SERIAL_LINE] = { 0 };	
		string DataFormat[NUM_MEASUREMENTS_PER_SERIAL_LINE] = { "Front Accel X", "Front Accel Y", "Front Accel Z", "Back Accel X", "Back Accel Y", "Back Accel Z",
																"Front Roll", "Front Pitch", "Front Yaw", "Back Roll", "Back Pitch", "Back Yaw" };
		bool print_val_flag = true;

		void DisplayData(void);
};


#endif