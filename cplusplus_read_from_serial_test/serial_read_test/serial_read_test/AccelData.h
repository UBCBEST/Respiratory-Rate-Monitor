// Class container for the accelerometer data. Using a singular class to store the acceleration data from both accelerometers 
// due to how the data is currently sent by the serial device and due to how its parsed
#ifndef ACCEL_DATA_H_INCLUDED
#define ACCEL_DATA_H_INCLUDED

#define MAX_DATA_LENGTH 255
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

class Accel_Data {
	public:
		Accel_Data();	// Default construtor
		//~Accel_Data();	// Default destructor
		bool Update_Data(char * DataFromSerial, int DataLength);
	private:
		// Currently somewhat arbitrary which set of measurements belong to which device
		double front_accel_x;
		double front_accel_y;
		double front_accel_z;
		double front_pitch;
		double front_roll;
		double front_yaw;
		double back_accel_x;
		double back_accel_y;
		double back_accel_z;
		double back_pitch;
		double back_roll;
		double back_yaw;
};


#endif