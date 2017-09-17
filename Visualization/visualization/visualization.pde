import processing.serial.*;
import controlP5.*;

Serial myPort;
String val;
float x_acc,y_acc,z_acc,roll,pitch,yaw;
PFont f;

void setup()
{
  size(1200,600, P3D);
  //setup serial port and communication between arduino and processing
  String portName = Serial.list()[0]; //depends on your serial port
  myPort = new Serial(this, portName, 9600); 
  f = createFont("Arial", 16, true);
  
  //Initialize data variables
  x_acc = 0.0;
  y_acc = 0.0;
  z_acc = 0.0;
  roll = 0.0;
  pitch = 0.0;
}

void draw()
{
    if ( myPort.available() > 0) 
    {  // If data is available,
      val = myPort.readStringUntil('\n');         // read serial data and store it in val
      String[] data = splitTokens(val, "\t");  //split up serial data and store in array of strings
    
      //cast each string into a float and store into appropriate variables
      x_acc = float(data[1]);
      y_acc = float(data[3]);
      z_acc = float(data[5]);
      roll = float(data[7]);
      pitch = float(data[9]);
    
      //Display obtained data on screen at the top
      background(100); //clear screen
      textFont(f,16);                  
      fill(255);                         
      text("X:  " + data[1] + "    Y: " + data[3] + "    Z: " + data[5] + 
      "    Roll: " + data[7] + "    Pitch: " + data[9],20,20);
      
      rectMode(CENTER);
      fill(51);
      stroke(255);
      translate(600, 300, 0);
      rotateX(radians(roll));
      rotateY(radians(-1*pitch));
      rect(0, 0, 300, 300);
    } 

}