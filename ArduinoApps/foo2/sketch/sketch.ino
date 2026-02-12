#include <Arduino_RouterBridge.h>
#include <Modulino.h>

/*
 * Modulino Buttons - Basic
 *
 * This example code is in the public domain.
 * Copyright (c) 2025 Arduino
 * SPDX-License-Identifier: MPL-2.0
 */

#include <Arduino_Modulino.h>

// Create a ModulinoButtons object
ModulinoButtons buttons;
ModulinoThermo thermo;

bool button_a = true;
bool button_b = true;
bool button_c = true;

bool waitForLinux(int intervalMs = 200, int timeoutMs = 30000)
{
  // Wait until Linux has started, or timeout.
  boolean started = false;
  unsigned long startTimeMs = millis();

  while (true)
  {
    if (Bridge.call("linux_started").result(started))
    {
      // The Linux side has started.
      // (It doesn't seem necessary to check the 'started' value, but we're using the chained '.result()' to test for call success.)
      started = true;
      break;
    }

    delay(intervalMs);

    if (millis() >= startTimeMs + timeoutMs)
    {
      // Timeout - stop waiting.
      break;
    }
  }
  printk("wait for Linux returned: %u\n", started);
  return started;
}

void setup() {
    Serial.begin(115200);
    printk("Setup Called\n");
    pinMode(LED_BUILTIN, OUTPUT);
    Bridge.begin();
    printk("After Bridge Begin\n");
    // Wait until Linux has started.
    if (!waitForLinux())
      return;
    delay(100);
    Monitor.begin();
    printk("After Monitor.begin\n");
    
    // Initialize Modulino I2C communication
    Modulino.begin(Wire1);
    // Detect and connect to buttons module
    printk("After Modulino begin\n");
    buttons.begin();
    // Turn on the LEDs above buttons A, B, and C
    printk("After buttons begin\n");
    buttons.setLeds(true, true, true);
    
    //thermo.begin();
}



void loop() {
  // Check for new button events, returns true when button state changes
  static uint32_t loop_count = 0;
  static uint32_t last_led_update_time = 0;
  if ((millis() - last_led_update_time) > 500) {
    loop_count++;
    digitalWrite(LED_BUILTIN, (loop_count & 1)? HIGH : LOW);
    last_led_update_time = millis();
  }
  Bridge.update();
  if (buttons.update()) {
    printk("buttons_update\n");
    // You can use either index (0=A, 1=B, 2=C) or letter ('A', 'B', 'C') to check buttons
    // Below we use the letter-based method for better readability

    if (buttons.isPressed('A')) {
      Serial.println("Button A pressed!");
      button_a = !button_a;
      Bridge.notify("btn_a_pressed", (int)button_a);
    } else if (buttons.isPressed("B")) {
      Serial.println("Button B pressed!");
      button_b = !button_b;
      Bridge.notify("btn_b_pressed", (int)button_b);
    } else if (buttons.isPressed('C')) {
      Serial.println("Button C pressed!");
      button_c = !button_c;
      Bridge.notify("btn_c_pressed", (int)button_c);
    }

    // Update the LEDs above buttons, depending on the variables value
    buttons.setLeds(button_a, button_b, button_c);
  }
}
