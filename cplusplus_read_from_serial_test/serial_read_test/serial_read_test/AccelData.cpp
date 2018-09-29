#include "AccelData.h"

// Initializes all member variables to 0
Accel_Data::Accel_Data() {
	this->front_accel_x = 0;
	this->front_accel_y	= 0;
	this->front_accel_z	= 0;
	this->front_pitch	= 0;
	this->front_roll	= 0;
	this->front_yaw		= 0;
	this->back_accel_x	= 0;
	this->back_accel_y	= 0;
	this->back_accel_z	= 0;
	this->back_pitch	= 0;
	this->back_roll		= 0;
	this->back_yaw		= 0;
}

// Takes the parsed string from the Serial read and tries to store and cast it into
// the member variables of the class container. 
// Returns true if successful
int Accel_Data::Update_Data(char * DataFromSerial, int DataLength) {
	char charBuffer[MAX_DATA_LENGTH] = "";
	char * pch; 

	if (DataLength == 0 || DataLength > MAX_DATA_LENGTH) {
		printf("Invalid input string size \n");
		return false;
	}

	do {
		pch = (char*)memchr(inputBuffer, ',', nbChar);	
	} while (pch != NULL);

	return true;
}

