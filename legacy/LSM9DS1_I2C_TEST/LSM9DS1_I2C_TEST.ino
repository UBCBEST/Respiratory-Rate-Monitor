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

////////////////////////////
// Sketch Output Settings //
////////////////////////////
#define PRINT_SPEED 50 // ms between prints
static unsigned long lastPrint = 0; // Keep track of print time

//http://www.magnetic-declination.com/
#define DECLINATION 16.23 // Declination (degrees) in Vancouver, Canada (16 deg, 14')

void setup() 
{
  
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
  // sensor_front.begin(), which verifies communication with the sensor_front
  // and turns it on.
  if (!sensor_front.begin())
  {
    Serial.println("Failed to communicate with front sensor.");
    Serial.println("Double-check wiring.");
    Serial.println("Default settings in this sketch will " \
                  "work for an out of the box LSM9DS1 " \
                  "Breakout, but may need to be modified " \
                  "if the board jumpers are.");
    while (1)
      ;
  }
  if (!sensor_back.begin())
  {
    Serial.println("Failed to communicate with back sensor.");
    Serial.println("Double-check wiring.");
    Serial.println("Default settings in this sketch will " \
                  "work for an out of the box LSM9DS1 " \
                  "Breakout, but may need to be modified " \
                  "if the board jumpers are.");
    while (1)
      ;
  }
}

void loop()
{
  if ( sensor_front.accelAvailable() )
  {
    sensor_front.readAccel();
  }
  if ( sensor_back.accelAvailable() )
  {
    sensor_back.readAccel();
  }
  if ( sensor_front.magAvailable() )
  {
    sensor_front.readMag();
  }
  if ( sensor_back.magAvailable() )
  {
    sensor_back.readMag();
  }
  
  if ((lastPrint + PRINT_SPEED) < millis())
  {
    printAccel();
    Serial.print(", ");
    printAttitude(sensor_front);
    Serial.print(", ");
    printAttitude(sensor_back);
    Serial.println();
    
    lastPrint = millis();
  }
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

void printAttitude(LSM9DS1 sensor)
{
  float ax = sensor.ax;
  float ay = sensor.ay;
  float az = sensor.az;
  float mx = sensor.mx;
  float my = sensor.my;
  float mz = sensor.mz;
  
  float roll = atan2(ay, az);
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));
  
  float heading;
  if (my == 0)
    heading = (mx < 0) ? PI : 0;
  else
    heading = atan2(mx, my);
    
  heading -= DECLINATION * PI / 180;
  
  if (heading > PI) heading -= (2 * PI);
  else if (heading < -PI) heading += (2 * PI);
  else if (heading < 0) heading += 2 * PI;
  
  // Convert everything from radians to degrees:
  heading *= 180.0 / PI;
  pitch *= 180.0 / PI;
  roll  *= 180.0 / PI;
  
  Serial.print(pitch, 3);
  Serial.print(", ");
  Serial.print(roll, 3);
  Serial.print(", ");
  Serial.print(heading, 3);
}
