/*
 Arduino example for MyViz tutorial 2 on Serial Port communication

 The LED is controlled by a MyViz dashboard.
 A value controlled by a slider in the MyViz dashboard is sent back to the dashboard.
 The values on A0 and A1 are displayed on this dashboard, after multiplication by a factor
 sent from a select control on the dashboard.

 Created 6 September 2016
 by Nicolas Gachadoit

 This example code is in the public domain.
 */

// Pins definitions:
const int LED = 13;

// Variables definitions
int onoff = 0; // On/off switch value (0 or 1)
float val = 0.0; // Value controlled by a slider (between 0 and 10)
int mult = 1; // Multiplication factor, controlled by a select widget (between 1 and 3)

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
    onoff = Serial.parseInt();
    // do it again:
    val = Serial.parseFloat();
    // do it again:
    mult = Serial.parseInt();

    // Look for the newline. That's the end of your
    // sentence:
    if (Serial.read() == '\r') {
      break;
    }
    
  }
  // Switch the LED
  digitalWrite(LED, onoff);
  
  // Read A0 and A1 and multiply them
  int multA0 = mult * analogRead(0);
  int multA1 = mult * analogRead(1);
  
  // Print these values separated with comma
  Serial.print(val);
  Serial.print(",");
  Serial.print(multA0);
  Serial.print(",");
  Serial.println(multA1);

  // Slight delay to avoid too much data
  delay(200);

}








