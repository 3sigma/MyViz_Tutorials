/*
 Arduino example for MyViz tutorial 1 on Serial Port communication

 The LED is controlled by a MyViz dashboard.
 A value controlled by a slider in the MyViz dashboard is sent back to the dashboard.
 The values on A0 and A1 are displayed on this dashboard.

 Created 13 July 2016
 by Nicolas Gachadoit

 This example code is in the public domain.
 */

// Pins définition:
const int LED = 13;

void setup() {
  // Initialize serial port:
  Serial.begin(9600);
  
  // Make the digital pins output:
  pinMode(LED, OUTPUT);

}

void loop() {
  // If there's any serial available, read it:
  while (Serial.available() > 0) {

    // Look for the next valid integer in the incoming serial stream:
    int onoff = Serial.parseInt(); // On/off switch value (0 or 1)
    // do it again:
    float val = Serial.parseFloat(); // Value controlled by a slider (between 0 and 10)

    // Look for the newline. That's the end of your
    // sentence:
    if (Serial.read() == '\r') {
      // Switch the LED
      digitalWrite(LED, onoff);
      
      // Read A0 and A1
      int a0 = analogRead(0);
      int a1 = analogRead(1);
      
      // Print these values separated with comma
      Serial.print(val);
      Serial.print(",");
      Serial.print(a0);
      Serial.print(",");
      Serial.println(a1);
    }
  }

}








