/*
 * Modulino Buttons - Basic
 *
 * This example code is in the public domain.
 * Copyright (c) 2025 Arduino
 * SPDX-License-Identifier: MPL-2.0
 */

#include <Arduino_RouterBridge.h>
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

#include <Arduino_Modulino.h>

// Create a ModulinoButtons object
ModulinoButtons buttons;
Arduino_LED_Matrix matrix;

bool button_a = true;
bool button_b = true;
bool button_c = true;

uint32_t blink_count = 0; 
uint32_t last_blink_time = 0;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  last_blink_time = millis();

  Monitor.begin();
  //Serial.begin(115200);
  matrix.begin();
  matrix.textFont(Font_5x7);
  matrix.textSize(1, 1);
  matrix.stroke(127, 127, 127);
  matrix.clear();

  // Initialize Modulino I2C communication
  Modulino.begin();
  // Detect and connect to buttons module
  buttons.begin();
  // Turn on the LEDs above buttons A, B, and C
  buttons.setLeds(true, true, true);

  display_string(":D");

}


void loop() {
  if ((millis() - last_blink_time) > 500) {
    digitalWrite(LED_BUILTIN, (++blink_count & 1)? HIGH : LOW);
    last_blink_time = millis();
  }
  // Check for new button events, returns true when button state changes
  if (buttons.update()) {
    // You can use either index (0=A, 1=B, 2=C) or letter ('A', 'B', 'C') to check buttons
    // Below we use the letter-based method for better readability

    if (buttons.isPressed('A')) {
      display_string("A");
      Monitor.println("Button A pressed!");
      button_a = !button_a;
    } else if (buttons.isPressed("B")) {
      display_string("B");
      Monitor.println("Button B pressed!");
      button_b = !button_b;
    } else if (buttons.isPressed('C')) {
      display_string("C");
      Monitor.println("Button C pressed!");
      button_c = !button_c;
    }

    // Update the LEDs above buttons, depending on the variables value
    buttons.setLeds(button_a, button_b, button_c);
  }
  delay(5000);
}

void display_string(const char* buffer) {
  Monitor.write(buffer);
  matrix.beginDraw();
  matrix.clear();
  int cch = strlen(buffer);
  if (cch <= 2) {
    matrix.textFont(Font_5x7);
    int x_start = (13 - cch * 5) / 2;
    matrix.text(buffer, (x_start < 0) ? 0 : x_start, 1);
  } else {
    matrix.textFont(Font_4x6);
    if ((cch == 4) && (buffer[0] == '0') && (buffer[1] == '.')) {
      matrix.text(&buffer[1], 0, 1);
    } else {
      matrix.text(buffer, 0, 1);
    }
  }

  matrix.endDraw();
}
