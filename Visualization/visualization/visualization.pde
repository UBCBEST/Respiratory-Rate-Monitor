 //<>// //<>//
//<<<<<<< HEAD
////hi
//=======
//>>>>>>> 97f210a6c9a74105442c787a0315af235d645ee1
import processing.serial.*;
import controlP5.*;
import signal.library.*;


// Data class for accelerometer data
public class AccelMeasurements{
  float m_x_acc_l, m_y_acc_l, m_z_acc_l, m_roll_l, m_pitch_l, m_yaw_l;
  
  public AccelMeasurements(){
  // constructor for the class, empty for now 
  }
  
  // Setter function for class members, takes accelerometer data and casts to a float 
  public void ImportData(String x_acc_l, String y_acc_l, String z_acc_l, String roll_l, String pitch_l, String yaw_l){
    this.m_x_acc_l = float(x_acc_l); 
    this.m_y_acc_l = float(y_acc_l);
    this.m_z_acc_l = float(z_acc_l);
    this.m_roll_l = float(roll_l);
    this.m_pitch_l = float(pitch_l);
    this.m_yaw_l = float(yaw_l);
  }
}  

Serial myPort;
AccelMeasurements AccelData_Front, AccelData_Back;
String val;
//float x_acc_1,y_acc_1,z_acc_1,roll_1,pitch_1,yaw_1;
PFont f;
ControlP5 cp5;
SignalFilter frontFilter, backFilter;
Chart myChart;

float minCutoff = 0.05;
float beta      = 4.0;
 
void setup()
{
  size(1200,600, P3D);
   //cp5 and chart objects
   cp5 = new ControlP5(this);
   frontFilter = new SignalFilter(this);
   backFilter = new SignalFilter(this);
   myChart = cp5.addChart("data")
               .setPosition(600, 50)
               .setSize(500, 300)
               .setRange(-0.5, 0.5)
               .setView(Chart.LINE) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
               .setStrokeWeight(15)
               .setColorCaptionLabel(color(40))
               .setResolution(100)
               ;
   
  myChart.addDataSet("incoming");
  myChart.setData("incoming", new float[100]);

  //setup serial port and communication between arduino and processing
  String portName = Serial.list()[0]; //depends on your serial port
  myPort = new Serial(this, portName, 115200); 
  AccelData_Front = new AccelMeasurements();
  AccelData_Back = new AccelMeasurements();
  f = createFont("Arial", 16, true);
  
  //make orthogonal view
  ortho(-width/2, width/2, -height/2, height/2);
  
  //Initialize data variables
  //x_acc_1 = 0.0;
  //y_acc_1 = 0.0;
  //z_acc_1 = 0.0;
  //roll_1 = 0.0;
  //pitch_1 = 0.0;
  //yaw_1 = 0.0;
   
  
  // Disable graph until we want to plot
  cp5.setAutoDraw(false); 
}

void draw()
{
    frontFilter.setMinCutoff(minCutoff);
    frontFilter.setBeta(beta);
    backFilter.setMinCutoff(minCutoff);
    backFilter.setBeta(beta);

    if ( myPort.available() > 0) 
    {  // If data is available,
      int lf = 10; 
      val = myPort.readStringUntil(lf);         // read serial data and store it in val
      String [] data; 
      
      if (val != null){
        data = splitTokens(val, ", ");  //split up serial data and store in array of strings
      }
      else{
        data = null; 
      }
      
      if (data != null){
        if (data.length == 12){
          //cast each string into a float and store into appropriate variables
          
          AccelData_Front.ImportData(data[0],data[1],data[2],data[6],data[7],data[8]);  // Currently just trying to duplicate the rotating box
          AccelData_Back.ImportData(data[3],data[4],data[5],data[9],data[10],data[11]);
          
          AccelData_Front.m_z_acc_l = frontFilter.filterUnitFloat( AccelData_Front.m_z_acc_l );
          AccelData_Back.m_z_acc_l = backFilter.filterUnitFloat( AccelData_Back.m_z_acc_l );
          //x_acc_1 = float(data[1]);
          //y_acc_1 = float(data[3]);
          //z_acc_1 = float(data[5]);
          
          //roll_1 = float(data[7]);
          //pitch_1 = float(data[9]);
          //yaw_1 = 0.0;
          
          //Display obtained data on screen at the top
          //TODO: Encapsulate the text printing into a function 
          background(100); //clear screen
          textFont(f,15);                  
          fill(255);                         
          text("X1:  " + data[0] + "    Y1: " + data[1] + "    Z1: " + AccelData_Front.m_z_acc_l + 
          "    Roll1: " + data[6] + "    Pitch1: " + data[7],20,20);
          text("X2:  " + data[3] + "    Y2: " + data[4] + "    Z2: " + AccelData_Back.m_z_acc_l + 
          "    Roll2: " + data[9] + "    Pitch2: " + data[10],20,50);
          
          //display accelerometer as 3D box
          //Note: The library will treat subsequent translation/rotation data as
          // an aggregate of the last input until the loop/cycle refreshes
          displayBox(AccelData_Front, 200, 150, 0, #ff0000);  // Accelerometer 1's visualization object
          displayBox(AccelData_Back, 200, 300, 0, 51);  // Accelerometer 2's visualization object
        
          // Enable chart and plot data
          camera(); 
          cp5.draw(); 
          plotChart();
        }
      }    
       
    } 

}

void displayBox (AccelMeasurements AccelData, int xPos, int yPos, int zPos, int rgb){
      pushMatrix();
      stroke(255);
      fill(rgb);
      translate(xPos, yPos, zPos);
      rotateX(radians(AccelData.m_roll_l));
      rotateY(radians(-1*AccelData.m_pitch_l));
      box(150, 150, 20);  
      popMatrix();
}


void plotChart(){
      myChart.push("incoming", AccelData_Front.m_z_acc_l + AccelData_Back.m_z_acc_l);
}   