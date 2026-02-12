#include <Arduino.h>
#line 1 "/home/arduino/ArduinoApps/bridge-speed-test/sketch/sketch.ino"
#include <Arduino_RouterBridge.h>
//String send_data = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
String send_data = "0123456789";

// A simple function to test round-trip latency. It does nothing.
#line 6 "/home/arduino/ArduinoApps/bridge-speed-test/sketch/sketch.ino"
String ping();
#line 10 "/home/arduino/ArduinoApps/bridge-speed-test/sketch/sketch.ino"
void setup();
#line 28 "/home/arduino/ArduinoApps/bridge-speed-test/sketch/sketch.ino"
void loop();
#line 6 "/home/arduino/ArduinoApps/bridge-speed-test/sketch/sketch.ino"
String ping() {
  return send_data;
}

void setup() {
    if (!Bridge.begin()) {
        Monitor.print("cannot setup Bridge\n"); delay(10);
    }

    if(!Monitor.begin()){
        Monitor.print("cannot setup Monitor\n"); delay(10);
    }

    delay(5000);
  
    if (!Bridge.provide("ping", ping)) {
        Monitor.print("Error providing method: ping\n"); delay(10);
    }

    Monitor.print("Setup DONE.\n"); delay(10);
}

void loop() {
  // The Bridge library handles incoming requests in the background.
  delay(100);
}
