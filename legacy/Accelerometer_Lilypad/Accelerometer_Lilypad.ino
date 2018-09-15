/*
  Blink once each time the superloop runs.
  Take x y z coordinates from accelerometer and print to serial monitor (then copy/paste to excel to analyze).
*/
const int X_PIN = 14;
const int Y_PIN = 15;
const int Z_PIN = 16;
const int SECOND = 1000;
const double zeroG = 512;
const double scale = 102.3;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);
  pinMode(Z_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
    double Xval;
    double Yval;
    double Zval;
    
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(100);

    Xval = analogRead(X_PIN);
    delay(1);                 /*The person whose code we based this off of reccomended a small delay between pin readings.  He said he wasn't sure why.*/
    Yval = analogRead(Y_PIN);
    delay(1);
    Zval = analogRead(Z_PIN);
    delay(1);

    Xval = (Xval-zeroG)/scale;
    Yval = (Yval-zeroG)/scale;
    Zval = (Zval-zeroG)/scale;
    
    Serial.print(Xval);
    Serial.print("\t");
    Serial.print(Yval);
    Serial.print("\t");
    Serial.println(Zval);
}
