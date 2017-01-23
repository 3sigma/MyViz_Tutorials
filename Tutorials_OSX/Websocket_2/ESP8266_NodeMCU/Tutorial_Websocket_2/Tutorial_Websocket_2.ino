/*
 ESP8266 (NodeMCU) example for MyViz tutorial 2 on Websocket communication

 The built-in LED is controlled by a switch in the MyViz dashboard.
 The value of A0 is displayed on this dashboard, after multiplication by a factor
 sent from a select control on the dashboard.
 A value controlled by a slider in the MyViz dashboard is sent back to the dashboard.
 
 Created 6 September 2016
 by Nicolas Gachadoit

 This example code is in the public domain.
 */

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>

const char* ssid = "MyViz";
const char* password = "myvizesp8266";

WebSocketsServer webSocket = WebSocketsServer(80);

// Switch value (for the LED) coming from MyViz
int onoff = 0;
// Slider value coming from MyViz
float val = 0;
// Multiplication factor coming from MyViz
int mult = 1;

// Called when a new message from the WebSocket is received
void handleClientData(String &dataString) {
  // Memory pool for JSON object tree.
  // Inside the brackets, 200 is the size of the pool in bytes,
  // If the JSON object is more complex, you need to increase this value.
  StaticJsonBuffer<200> jsonBuffer;

  // Root of the object tree.
  //
  // It's a reference to the JsonObject, the actual bytes are inside the
  // JsonBuffer with all the other nodes of the object tree.
  // Memory is freed when jsonBuffer goes out of scope.
  JsonObject& root = jsonBuffer.parseObject(dataString);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do root["time"].as<long>();
  if (dataString.indexOf("\"onoff\"") > 0) {
    onoff = root["onoff"];
    // Switch the LED
    digitalWrite(BUILTIN_LED, !onoff);
  }
  if (dataString.indexOf("\"val\"") > 0) {
    val = root["val"];
  }
  if (dataString.indexOf("\"mult\"") > 0) {
    mult = root["mult"];
  }

  // Send values on UART (used only for debugging)
  Serial.print(onoff);
  Serial.print(",");
  Serial.print(val);
  Serial.print(",");
  Serial.print(mult);
  Serial.print("\r");
  Serial.print("\n");

}

// Send to the client the value of multiplied A0 and the value of "val" received
void sendClientData(uint8_t num) {
  String data;

  // Memory pool for JSON object tree.
  //
  // Inside the brackets, 200 is the size of the pool in bytes.
  // If the JSON object is more complex, you need to increase this value.
  StaticJsonBuffer<200> jsonBuffer;

  // Create the root of the object tree.
  //
  // It's a reference to the JsonObject, the actual bytes are inside the
  // JsonBuffer with all the other nodes of the object tree.
  // Memory is freed when jsonBuffer goes out of scope.
  JsonObject& root = jsonBuffer.createObject();

  // Add values in the object
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do root.set<long>("time", 1351824120);
  root["retval"] = val;
  root["multA0"] = mult * analogRead(A0);

  // For debugging only
  //root.prettyPrintTo(Serial);

  root.printTo(data);

  webSocket.sendTXT(num, data);

}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
      
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;
      
    case WStype_TEXT:
      {
        //Serial.printf("[%u] get Text: %s\n", num, payload);
  
        String data;
        data = String((char*)payload);
        handleClientData(data);
  
        sendClientData(num);
  
        // send message to client
        // webSocket.sendTXT(num, "message here");
  
        // send data to all connected clients
        // webSocket.broadcastTXT("message here");
      }
      break;
      
    case WStype_BIN:
      {
        Serial.printf("[%u] get binary lenght: %u\n", num, lenght);
        hexdump(payload, lenght);
  
        // send message to client
        // webSocket.sendBIN(num, payload, lenght);
      }
      break;
  }

}

void setup() {

  Serial.begin(115200);

  pinMode(BUILTIN_LED, OUTPUT);

  Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for(uint8_t t = 4; t > 0; t--) {
      Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
      Serial.flush();
      delay(1000);
  }

  Serial.println("\nConfiguring access point...");
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(myIP);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

}

void loop() {
  webSocket.loop();
}
