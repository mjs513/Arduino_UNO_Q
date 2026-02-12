// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
//
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_RouterBridge.h>
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include <vector>

#include <Modulino.h>
#include <Arduino_Modulino.h>
#include "weather_frames.h"

#if __has_include(".location.h")
#include ".location.h";
#else
String city = "Los Angeles";
#endif

// Create a ModulinoButtons object
ModulinoButtons buttons;

bool button_a = true;
bool button_b = true;
bool button_c = true;
bool buttons_found = false;

// Also try a Qwiic button
#include <SparkFun_Qwiic_Button.h>
QwiicButton qbutton;
bool qbutton_found;

Arduino_LED_Matrix matrix;

#define MIN_SPEED_TO_DISPLAY 2.0

static const char* wind_directions[] = { "N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE",
                                         "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW" };

enum { TEMP = 0,
       PRECIP,
       SPEED,
       DIR };
void setup() {

  matrix.begin();
  matrix.textFont(Font_5x7);
  matrix.textSize(1, 1);
  matrix.stroke(127, 127, 127);
  matrix.clear();
  Bridge.begin();
  Monitor.begin();

  // Initialize Modulino I2C communication
  Modulino.begin(Wire1);
  // Detect and connect to buttons module
  printk("After Modulino begin\n");
  buttons_found = buttons.begin();
  // Turn on the LEDs above buttons A, B, and C
  printk("After buttons begin %u\n", buttons_found);
  if (buttons_found) buttons.setLeds(true, true, true);

  qbutton_found = qbutton.begin(SFE_QWIIC_BUTTON_DEFAULT_ADDRESS, Wire1);
  printk("QButton begin: %u\n", qbutton_found);
  if (qbutton_found && button_a) qbutton.LEDon(64);
}




/* make global so I can decide not to call through bridge every call */

String weather_forecast;
std::vector<float> cur_weather_data;

uint32_t last_forcast_time = (uint32_t)-1;
#define TIME_BETWEEN_CALLS_MS (5 * 60000) /* 5 minutes */

void loop() {
  if (buttons.update()) {
    // You can use either index (0=A, 1=B, 2=C) or letter ('A', 'B', 'C') to check buttons
    // Below we use the letter-based method for better readability

    if (buttons.isPressed('A')) {
      Serial.println("Button A pressed!");
      button_a = !button_a;
    } else if (buttons.isPressed("B")) {
      Serial.println("Button B pressed!");
      button_b = !button_b;
    } else if (buttons.isPressed('C')) {
      Serial.println("Button C pressed!");
      button_c = !button_c;
    }

    // Update the LEDs above buttons, depending on the variables value
    buttons.setLeds(button_a, button_b, button_c);
  }

  if (qbutton_found && qbutton.hasBeenClicked()) {
      button_a = !button_a;
      printk("QButton pressed: %u\n", button_a);
      if (button_a) qbutton.LEDon(64);
      else qbutton.LEDoff();
      qbutton.clearEventBits();
  }

  if ((last_forcast_time == (uint32_t)-1) || ((millis() - last_forcast_time) > TIME_BETWEEN_CALLS_MS)) {
    bool ok = Bridge.call("get_weather_forecast", city).result(weather_forecast);
    if (!ok) {
      delay(5000);
      Monitor.println("Failed to get_weather_forecast");
      return;
    }
    Bridge.call("get_weather_data").result(cur_weather_data);
  }
  char buffer[80];
  sprintf(buffer, "%f.1 %f.1 %f.1 %f,1 ", cur_weather_data[TEMP], cur_weather_data[PRECIP], cur_weather_data[SPEED], cur_weather_data[DIR]);
  Monitor.print(buffer);

  if (weather_forecast == "sunny") {
    matrix.loadSequence(sunny);
    playRepeat(5);
  } else if (weather_forecast == "cloudy") {
    matrix.loadSequence(cloudy);
    playRepeat(5);
  } else if (weather_forecast == "rainy") {
    matrix.loadSequence(rainy);
    playRepeat(10);
  } else if (weather_forecast == "snowy") {
    matrix.loadSequence(snowy);
    playRepeat(5);
  } else if (weather_forecast == "foggy") {
    matrix.loadSequence(foggy);
    playRepeat(3);
  }

  // lets show temp
  if (!button_a) {
    sprintf(buffer, "%d", (int)(cur_weather_data[TEMP] + 0.5));
    display_string(buffer);
    delay(2000);

    // maybe show precip
    //Monitor.print(cur_precip);
    if (cur_weather_data[PRECIP] > 0) {
      if (cur_weather_data[PRECIP] >= 1.0) {
        sprintf(buffer, "%d", (int)cur_weather_data[PRECIP]);
      } else if (cur_weather_data[PRECIP] >= 0.10) {
        sprintf(buffer, "%f.1", cur_weather_data[PRECIP]);
      } else {
        sprintf(buffer, "%f.2", cur_weather_data[PRECIP]);
      }
      display_string(buffer);
      delay(2000);
    }
    //Monitor.println();

    if (cur_weather_data[SPEED] >= MIN_SPEED_TO_DISPLAY) {
      // first display wind direction
      int dir_index = int((cur_weather_data[DIR] + 12.25) / 22.5);
      display_string(wind_directions[(dir_index < 16) ? dir_index : 0]);
      delay(2000);
      sprintf(buffer, "%d", (int)cur_weather_data[SPEED]);
      display_string(buffer);
      delay(2000);
    }
  } else {
    // Lets try a quick and dirty scrolling of text
    matrix.textFont(Font_5x7);
    matrix.textScrollSpeed(100);
    matrix.clear();
    matrix.beginText(0, 0, 127, 0, 0);  // X, Y, then R, G, B
    sprintf(buffer, "  T %d", (int)(cur_weather_data[TEMP] + 0.5));
    matrix.print(buffer);
    if (cur_weather_data[PRECIP] > 0) {
      sprintf(buffer, " R %f.1");
      matrix.print(buffer);
    }
    if (cur_weather_data[SPEED] >= MIN_SPEED_TO_DISPLAY) {
      // first display wind direction
      int dir_index = int((cur_weather_data[DIR] + 12.25) / 22.5);
      matrix.print(" W ");
      matrix.print(wind_directions[(dir_index < 16) ? dir_index : 0]);
      sprintf(buffer, " %d", (int)cur_weather_data[SPEED]);
      matrix.print(buffer);
    }
    // now try to scroll this text
    matrix.endText(SCROLL_LEFT);
  }
}

void playRepeat(int repeat_count) {
  for (int i = 0; i < repeat_count; i++) {
    matrix.playSequence();
  }
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
