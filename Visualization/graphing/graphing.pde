import processing.serial.*;
import controlP5.*;

Serial myPort;
String val;
float x_acc_1,y_acc_1,z_acc_1,roll_1,pitch_1,yaw_1;
PFont f;
ControlP5 cp5;
Chart myChart;

String ButtonActive, ButtonPressed, PrevButtonPressed;
Button X1, Y1;

void setup()
{
  size(1200,600);
   //cp5 and chart objects
   cp5 = new ControlP5(this);
   myChart = cp5.addChart("data")
               .setPosition(600, 50)
               .setSize(500, 300)
               .setRange(-10, 10)
               .setView(Chart.LINE) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
               .setStrokeWeight(15)
               .setColorCaptionLabel(color(40))
               .setResolution(100)
               ;
       
  myChart.addDataSet("incoming");
  myChart.setData("incoming", new float[100]);
  
  ButtonActive = "X1";
  
  // create a new button with name 'X1'
  X1 = cp5.addButton("X1")
     .setValue(0)
     .setPosition(500,50)
     .setSize(80,15)
     .setSwitch(true);
     ;
  
    // create a new button with name 'Y1'
  Y1 = cp5.addButton("Y1")
     .setValue(0)
     .setPosition(500,80)
     .setSize(80,15)
     .setSwitch(true);
     ;
  
  //setup serial port and communication between arduino and processing
  String portName = Serial.list()[0]; //depends on your serial port
  myPort = new Serial(this, portName, 9600); 
  f = createFont("Arial", 16, true);
  
  //Initialize data variables
  x_acc_1 = 0.0;
  y_acc_1 = 0.0;
  z_acc_1 = 0.0;
  roll_1 = 0.0;
  pitch_1 = 0.0;
  yaw_1 = 0.0;
  
  X1.setOn();
  
}

void draw()
{
    if ( myPort.available() > 0) 
    {  // If data is available,
      val = myPort.readStringUntil('\n');         // read serial data and store it in val
      String[] data = splitTokens(val, "\t");  //split up serial data and store in array of strings
    
      //cast each string into a float and store into appropriate variables
      x_acc_1 = float(data[1]);
      y_acc_1 = float(data[3]);
      z_acc_1 = float(data[5]);
      roll_1 = float(data[7]);
      pitch_1 = float(data[9]);
      yaw_1 = 0.0;
      
      //Display obtained data on screen at the top
      background(100); //clear screen
      textFont(f,15);                  
      fill(255);                         
      text("X1:  " + data[1] + "    Y1: " + data[3] + "    Z1: " + data[5] + 
      "    Roll1: " + data[7] + "    Pitch1: " + data[9],20,20);
      
      plotChart();
     
    }
}

public void controlEvent(ControlEvent theEvent) {
  
  ButtonPressed = theEvent.getController().getName();
  
  
  if(X1.isOn() && !Y1.isOn()){
    ButtonActive = "X1";
  }
  
  if(Y1.isOn() && !X1.isOn()){
    ButtonActive = "Y1";
  }
  
  if(!Y1.isOn() && !X1.isOn()){
    ButtonActive = "None";
  }
}

void plotChart(){
  
  switch(ButtonActive){
    case "X1":
      myChart.push("incoming", x_acc_1);
      Y1.setOff();
      break;
    case "Y1":
      myChart.push("incoming", y_acc_1);
      X1.setOff();
    case "None":
      myChart.push("incoming", 0);
    
  }
}