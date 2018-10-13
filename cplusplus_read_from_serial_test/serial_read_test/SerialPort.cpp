#include "SerialPort.h"

Serial::Serial(const char *portName) {
	// Not connected by default
	this->connected = false;

	// Attempt to connect to the given port by calling CreateFile
	this->hSerial = CreateFile(portName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//Check if the connection was successfull
	if (this->hSerial == INVALID_HANDLE_VALUE) {
		//If not success full display an Error
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			//Print Error if neccessary
			printf("ERROR: Handle was not attached. Reason: %s not available.\n", portName);
		}
		else {
			printf("ERROR!!!");
		}
	}
	else {
		//If connected we try to set the comm parameters
		DCB dcbSerialParams = { 0 };

		Accel_Data AccelData;	

		//Try to get the current serial connection parameters
		if (!GetCommState(this->hSerial, &dcbSerialParams)) {
			// Print an error if this fails 
			printf("Failed to get current serial parameters!");
		}
		else {
			//Define serial connection parameters for the arduino board
			dcbSerialParams.BaudRate = CBR_115200;
			dcbSerialParams.ByteSize = 8;
			dcbSerialParams.StopBits = ONESTOPBIT;
			dcbSerialParams.Parity = NOPARITY;
			// Setting the DTR to Control_Enable ensures that the serial device is properly
			// reset upon establishing a connection
			dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

			//Set the parameters and check for their proper application
			if (!SetCommState(hSerial, &dcbSerialParams)) {
				printf("ALERT: Could not set Serial Port parameters");
			}
			else {
				// Connection made with serial device 
				this->connected = true;
				// Flush any remaining characters in the buffers 
				PurgeComm(this->hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
				// Wait to allow the serial device to reset 
				Sleep(ARDUINO_WAIT_TIME);
			}
		}
	}
}

Serial::~Serial()
{
	//Check if we are connected before trying to disconnect
	if (this->connected) {
		//We're no longer connected
		this->connected = false;
		//Close the serial handler
		CloseHandle(this->hSerial);
		
		this->AccelData.~AccelData();
	}
}

// Currently this function takes an int nbChar as a fixed number of bytes to read from the Serial device
int Serial::ReadData(char *buffer, unsigned int nbChar)
{
	//Number of bytes we'll have read
	DWORD bytesRead;
	//Number of bytes we'll really ask to read
	unsigned int toRead;

	//Use the ClearCommError function to get status info on the Serial port
	ClearCommError(this->hSerial, &this->errors, &this->status);

	//Check if there is something to read
	if (this->status.cbInQue>0) {
		//If there is we check if there is enough data to read the required number
		//of characters, if not we'll read only the available characters to prevent
		//locking of the application.
		if (this->status.cbInQue>nbChar) {
			toRead = nbChar;
		}
		else {
			toRead = this->status.cbInQue;
		}

		//Try to read the requested number of chars, and return the number of read bytes on success
		if (ReadFile(this->hSerial, buffer, toRead, &bytesRead, NULL)) {
			return bytesRead;
		}
	}
	//If nothing has been read, or that an error was detected return 0
	return 0;
}

// Takes the character buffer from ReadFile and the number of bytes read, parses the buffer until the desired end of line 
// character is reached. When its reached then print it. Move the remainder of the string into a private class member buffer
void Serial::ParseRead(char *inputBuffer, int nbChar)
{
	char outputBuffer[256] = ""; 
	int inBufEOL_pos;	
	int ParseBuf_len = strlen(ParseBuffer); // size of our parse buffer
	char * pch = (char*) memchr(inputBuffer, '\n', nbChar);	// Get the pointer to the end-of-line character
	
	//printf("STARTING STRING is %s \n", inputBuffer);

	if (pch != NULL) {	// We found the endofline character 
		inBufEOL_pos = (pch - inputBuffer + 1);	// This returns the position NOT the index of the end of line character
		//printf("EOL CHARACTER FOUND AT %d POSITION \n", inBufEOL_pos);
		if (ParseBuf_len) {		// Check if our parse buffer contains contents from the previous read
			// Currently we aren't worried about overflow (though this could be a good improvement in the future)
			strncat_s(ParseBuffer, inputBuffer, inBufEOL_pos);	// Concatonate on our parse buffer
			printf("%s\n", ParseBuffer);						// Our parse buffer contains enough content to print

			this->AccelData.Update_Data(ParseBuffer, strlen(ParseBuffer));	// Update accel data class member data

			memset(ParseBuffer, 0, strlen(ParseBuffer));		// Contents have been printed, clear the buffer 

			if (nbChar > inBufEOL_pos) {	// If we have any remaining characters in the input (which we will)
				memcpy(outputBuffer, &inputBuffer[inBufEOL_pos], (nbChar - inBufEOL_pos));	// Copy the remainder of the input into the buffer
				ParseRead(outputBuffer, (nbChar - inBufEOL_pos));	// Recursively call this function
			}
		}
		else { // Its empty. We can directly copy to our output buffer
			strncpy_s(outputBuffer, inputBuffer, inBufEOL_pos);

			printf("%s\n", outputBuffer);		// Print the contents	
			this->AccelData.Update_Data(outputBuffer, strlen(outputBuffer));	// Update accel data class member data

			if (nbChar > inBufEOL_pos) {	// If we have any remaining characters in the input (which we will)
				memset(outputBuffer, 0, strlen(outputBuffer));		// Contents have been printed, clear the buffer 
				memcpy(outputBuffer, &inputBuffer[inBufEOL_pos], (nbChar - inBufEOL_pos));	// Copy the remainder of the input into the buffer
				ParseRead(outputBuffer, (nbChar - inBufEOL_pos));	// Recursively call this function
			}
		}

	}
	else {				// No end of line character, store our data into our buffer
		//printf("NO END OF LINE CHARACTER FOUND IN 255 BYTES \n");
		strncat_s(ParseBuffer, inputBuffer, nbChar);
	}
}

bool Serial::WriteData(const char *buffer, unsigned int nbChar)
{
	DWORD bytesSend;

	//Try to write the buffer on the Serial port
	if (!WriteFile(this->hSerial, (void *)buffer, nbChar, &bytesSend, 0)) {
		//In case it doesn't work get comm error and return false
		ClearCommError(this->hSerial, &this->errors, &this->status);

		return false;
	}
	else
		return true;
}

bool Serial::IsConnected()
{
	//Simply return the connection status
	return this->connected;
}