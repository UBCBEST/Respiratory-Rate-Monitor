#include "AccelData.h"

// Currently empty
Accel_Data::Accel_Data() {
}

// Setter function to toggle if we want to print our values to the VS console
void Accel_Data::set_print_val_flag(bool flag_val) {
	this->print_val_flag = flag_val;
}

// Takes the parsed string from the Serial read and tries to store and cast it into
// the member variables of the class container. 
// Returns true if successful
bool Accel_Data::Update_Data(char * DataFromSerial, int DataLength) {
	if (DataLength == 0 || DataLength > MAX_DATA_LENGTH) {
		printf("Invalid input string size \n");
		return false;
	}

	double ParseDataBuffer[NUM_MEASUREMENTS_PER_SERIAL_LINE+1] = { 0 };	// Adding an extra element to array as our for loop later will attempt to write to this on its failing loop (else will return a stack corruption err)
	int numValsParsed = 0;		// Keep a counter for the number of values parsed
	char * copyBuffer = DataFromSerial;	// Doesn't matter if we point to our original buffer since we clear it anyways

	// First remove all commas from our string to make it easily digestable by the strtod function
	for (int i = 0; i < DataLength; i++) 
		if (copyBuffer[i] == ',') copyBuffer[i] = ' ';
	copyBuffer[DataLength] = '\0';	// Append a null character to the end of our string

	char * endptr;	// Gets updated to point at the first character after the number in the strtod func
	// Parse our char array into a temporary array of doubles. If copyBuffer and endptr point to the same location 
	// that indicates the strtod has failed hence there is no more data to parse
	for ((ParseDataBuffer[numValsParsed] = strtod(copyBuffer, &endptr)); copyBuffer != endptr; (ParseDataBuffer[numValsParsed] = strtod(copyBuffer, &endptr))) {
		copyBuffer = endptr;
		numValsParsed++;
		if (numValsParsed > NUM_MEASUREMENTS_PER_SERIAL_LINE) {
			printf("Error during parsing. Either a malformed string or a non-numeral/signed value in buffer \n");
			return false;
		}
	}

	// Parsed serial string had the valid number of data values
	if (numValsParsed == NUM_MEASUREMENTS_PER_SERIAL_LINE) {
		copy(ParseDataBuffer, ParseDataBuffer + NUM_MEASUREMENTS_PER_SERIAL_LINE, this->accelData);	// Copy buffer data into our class member data array
		if (print_val_flag)	// Print formatted results 
			this->DisplayData();
	}
	else
		return false;	// String given by parsing function was malformed

	return true;
}

// Function to print the formatted accelerometer readings
void Accel_Data::DisplayData(void) {
	for (int i = 0; i < NUM_MEASUREMENTS_PER_SERIAL_LINE; i++) {
		cout << DataFormat[i] << ": ";
		cout << left << setw(8) << accelData[i];
		if (i == 5)
			cout << endl;
	}
	cout << endl;
}
