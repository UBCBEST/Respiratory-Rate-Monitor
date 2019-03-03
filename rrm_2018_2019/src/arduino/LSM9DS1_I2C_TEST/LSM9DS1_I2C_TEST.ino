/*****************************************************************
LSM9DS1_Basic_I2C.ino

Connection for both sensors to Arduino:
	LSM9DS1 -------- Arduino
	 SCL ------------- SCL (A5 on older 'Duinos')
	 SDA ------------- SDA (A4 on older 'Duinos')
	 VDD ------------- 3.3V
	 GND ------------- GND

SDO_M and SDO_AG must be pulled down to GND on one of the sensors
*****************************************************************/
// The SFE_LSM9DS1 library requires both Wire and SPI be
// included BEFORE including the 9DS1 library.
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>

//////////////////////////
// LSM9DS1 Library Init //
//////////////////////////

LSM9DS1 sensor_front;
LSM9DS1 sensor_back;

///////////////////////
// Example I2C Setup //
///////////////////////
// SDO_XM and SDO_G are both pulled high, so our addresses are:
#define MAG_FRONT	0x1E 
#define ACC_FRONT	0x6B 
#define MAG_BACK  0x1C // SDO_M pulled down
#define ACC_BACK  0x6A // SDO_AG pulled down

boolean printSample = true;

//http://www.magnetic-declination.com/
#define DECLINATION 16.23 // Declination (degrees) in Vancouver, Canada (16 deg, 14')

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  
  // Before initializing the sensor_front, there are a few settings
  // we may need to adjust. Use the settings struct to set
  // the device's communication mode and addresses:
  sensor_front.settings.device.commInterface = IMU_MODE_I2C;
  sensor_front.settings.device.mAddress = MAG_FRONT;
  sensor_front.settings.device.agAddress = ACC_FRONT;
  sensor_back.settings.device.commInterface = IMU_MODE_I2C;
  sensor_back.settings.device.mAddress = MAG_BACK;
  sensor_back.settings.device.agAddress = ACC_BACK;
  // The above lines will only take effect AFTER calling
  // sensor_front.begin() and sensor_back.begin(), which verifies 
  // communication with the sensor_front and turns it on.
  if (!sensor_front.begin())
  {
    Serial.println("Failed to communicate with front sensor.");
    Serial.println("Double-check wiring.");
    Serial.println("Default settings in this sketch will " \
                  "work for an out of the box LSM9DS1 " \
                  "Breakout, but may need to be modified " \
                  "if the board jumpers are.");
    while (1);
  }
  if (!sensor_back.begin())
  {
    Serial.println("Failed to communicate with back sensor.");
    Serial.println("Double-check wiring.");
    Serial.println("Default settings in this sketch will " \
                  "work for an out of the box LSM9DS1 " \
                  "Breakout, but may need to be modified " \
                  "if the board jumpers are.");
    while (1);
  }
  
  cli(); // stop interrupts

  //set timer1 interrupt at 50Hz
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; //initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 1249; // = (16*10^6) / (50*64) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 64 prescaler
  TCCR1B |= (1 << CS01) | (1 << CS00); 
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  sei(); // allow interrupts
}

// Timer1 interrupt triggers at 50Hz
ISR(TIMER1_COMPA_vect){
  printSample = true; // Timer ISR trigger data transmission
}

void loop()
{ 
  if(printSample) 
  { 
    
    if ( sensor_front.gyroAvailable() )   sensor_front.readGyro();
    if ( sensor_back.gyroAvailable() )    sensor_back.readGyro();
    if ( sensor_front.accelAvailable() )  sensor_front.readAccel();
    if ( sensor_back.accelAvailable() )   sensor_back.readAccel();
    if ( sensor_front.magAvailable() )    sensor_front.readMag();
    if ( sensor_back.magAvailable() )     sensor_back.readMag();
    
    printGyro();
    Serial.print(", ");
    printAccel();
    Serial.print(", ");
    printMag();
    Serial.println();     // Separates sets of readings with a new line and carriage character
    printSample = false;
  }
}

// The following three functions format the data into human readable strings to send over serial communications
// Here new values are separated by a comma and space
void printGyro()
{
  Serial.print(sensor_front.calcGyro(sensor_front.gx), 3);
  Serial.print(", ");
  Serial.print(sensor_front.calcGyro(sensor_front.gy), 3);
  Serial.print(", ");
  Serial.print(sensor_front.calcGyro(sensor_front.gz), 3);
  Serial.print(", ");
  Serial.print(sensor_back.calcGyro(sensor_back.gx), 3);
  Serial.print(", ");
  Serial.print(sensor_back.calcGyro(sensor_back.gy), 3);
  Serial.print(", ");
  Serial.print(sensor_back.calcGyro(sensor_back.gz), 3);
}

void printAccel()
{  
  Serial.print(sensor_front.calcAccel(sensor_front.ax), 3);
  Serial.print(", ");
  Serial.print(sensor_front.calcAccel(sensor_front.ay), 3);
  Serial.print(", ");
  Serial.print(sensor_front.calcAccel(sensor_front.az), 3);
  Serial.print(", ");
  Serial.print(sensor_back.calcAccel(sensor_back.ax), 3);
  Serial.print(", ");
  Serial.print(sensor_back.calcAccel(sensor_back.ay), 3);
  Serial.print(", ");
  Serial.print(sensor_back.calcAccel(sensor_back.az), 3);
}

void printMag()
{ 
  Serial.print(sensor_front.calcMag(sensor_front.mx), 3);
  Serial.print(", ");
  Serial.print(sensor_front.calcMag(sensor_front.my), 3);
  Serial.print(", ");
  Serial.print(sensor_front.calcMag(sensor_front.mz), 3);
  Serial.print(", ");
  Serial.print(sensor_back.calcMag(sensor_back.mx), 3);
  Serial.print(", ");
  Serial.print(sensor_back.calcMag(sensor_back.my), 3);
  Serial.print(", ");
  Serial.print(sensor_back.calcMag(sensor_back.mz), 3);
}
