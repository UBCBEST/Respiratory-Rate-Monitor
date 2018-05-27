// Import libraries //<>//
import processing.serial.*;
import controlP5.*;
import signal.library.*;


// Data class for accelerometer data
public class AccelData{
  float m_x_acc, m_y_acc, m_z_acc, m_roll, m_pitch, m_yaw;
  
  public AccelData(){
  // constructor for the class, empty for now 
  }
  
  // Setter function for class members, takes accelerometer data and casts to a float 
  public void ImportData(String x_acc, String y_acc, String z_acc, String roll, String pitch, String yaw){
    this.m_x_acc   = float(x_acc); 
    this.m_y_acc   = float(y_acc);
    this.m_z_acc   = float(z_acc);
    this.m_roll    = float(roll);
    this.m_pitch   = float(pitch);
    this.m_yaw     = float(yaw);
  }
}  

// Variable and Object declarations
Serial myPort;
AccelData AccelData_Front, AccelData_Back;
String val;
PFont f;
ControlP5 cp5;
SignalFilter frontFilter, backFilter;
Chart myChart;

// Parameters for signal filter
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
               .setRange(-0.5, 0.5)  // Scale is in Gs
               .setView(Chart.LINE) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
               .setStrokeWeight(15)
               .setColorCaptionLabel(color(40))
               .setResolution(100)
               ;
   
  myChart.addDataSet("incoming");
  myChart.setData("incoming", new float[100]);

  // Setup serial port and communication between arduino and processing
  String portName = Serial.list()[0]; //depends on your serial port
  myPort = new Serial(this, portName, 115200); 
 
  AccelData_Front = new AccelData();
  AccelData_Back = new AccelData();
  f = createFont("Arial", 16, true);
  
  //make orthogonal view
  ortho(-width/2, width/2, -height/2, height/2);
  
  // Disable graph until we want to plot
  cp5.setAutoDraw(false); 
}

void draw()
{
  // Pass filter parameters into filter class objects
  frontFilter.setMinCutoff(minCutoff);
  frontFilter.setBeta(beta);
  backFilter.setMinCutoff(minCutoff);
  backFilter.setBeta(beta);

  if ( myPort.available() > 0) 
  {  // If data is available,
    int lf = 10;  
    val = myPort.readStringUntil(lf);  // Read and store serial data from Arduino, stop at 'Line Feed' character
    String [] data; 
      
    if (val != null){
      data = splitTokens(val, ", ");   // Store serial data into an array of strings
    }
    else{
      data = null; 
    }
      
    if (data != null){
      if (data.length == 12){  
        // Import serial data into acceleration class object members
        AccelData_Front.ImportData(data[0],data[1],data[2],data[6],data[7],data[8]); 
        AccelData_Back.ImportData(data[3],data[4],data[5],data[9],data[10],data[11]);
          
        // Apply filtering to acceleration data in the Z-axis
        AccelData_Front.m_z_acc = frontFilter.filterUnitFloat( AccelData_Front.m_z_acc );
        AccelData_Back.m_z_acc = backFilter.filterUnitFloat( AccelData_Back.m_z_acc );

        // Display serial data on the top left hand side of the screen 
        displayData(20,20,20,50);
          
        // Display accelerometer as 3D box
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

void displayBox (AccelData AccelData, int xPos, int yPos, int zPos, int rgb){
  pushMatrix();
  stroke(255);
  fill(rgb);
  translate(xPos, yPos, zPos);
  rotateX(radians(AccelData.m_roll));
  rotateY(radians(-1*AccelData.m_pitch));
  box(150, 150, 20);  
  popMatrix();
}

void displayData( int x_pos1, int y_pos1, int x_pos2, int y_pos2){
  background(100); //clear screen
  textFont(f,15);                  
  fill(255);                         
  text("X1:  " + AccelData_Front.m_x_acc + "    Y1: " + AccelData_Front.m_y_acc + "    Z1: " + AccelData_Front.m_z_acc + 
  "    Roll1: " + AccelData_Front.m_roll + "    Pitch1: " + AccelData_Front.m_pitch, x_pos1, y_pos1);
  text("X2:  " + AccelData_Back.m_x_acc + "    Y2: " + AccelData_Back.m_y_acc + "    Z2: " + AccelData_Back.m_z_acc + 
  "    Roll2: " + AccelData_Back.m_roll + "    Pitch2: " + AccelData_Back.m_pitch, x_pos2, y_pos2);
}

// Graph the difference between the Z-axis acceleration of the two accelerometers
void plotChart(){
  myChart.push("incoming", AccelData_Front.m_z_acc);
}   
