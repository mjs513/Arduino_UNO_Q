#include <Arduino_RouterBridge.h>
#include <Arduino_LED_Matrix.h>

uint8_t gradation[104] = {
  7,7,7,7,7,7,7,7,7,7,7,7,7,
  6,6,6,6,6,6,6,6,6,6,6,6,6,
  5,5,5,5,5,5,5,5,5,5,5,5,5,
  4,4,4,4,4,4,4,4,4,4,4,4,4,
  3,3,3,3,3,3,3,3,3,3,3,3,3,
  2,2,2,2,2,2,2,2,2,2,2,2,2,
  1,1,1,1,1,1,1,1,1,1,1,1,1,
  0,0,0,0,0,0,0,0,0,0,0,0,0
};

uint8_t logo[104] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,7,7,7,0,0,0,7,7,7,0,0,
  0,7,0,0,0,7,0,7,0,0,0,7,0,
  7,0,0,0,0,0,7,0,0,7,0,0,7,
  7,0,7,7,7,0,7,0,7,7,7,0,7,
  7,0,0,0,0,0,7,0,0,7,0,0,7,
  0,7,0,0,0,7,0,7,0,0,0,7,0,
  0,0,7,7,7,0,0,0,7,7,7,0,0
};

Arduino_LED_Matrix matrix;

void clear_led() {
  matrix.clear();
}

void show_gradation() {
  matrix.draw(gradation);
}

void show_logo() {
  matrix.draw(logo);
}

void setup() {
  Bridge.begin();
  Bridge.provide("clear_led", clear_led);
  Bridge.provide("show_gradation", show_gradation);
  Bridge.provide("show_logo", show_logo);
  matrix.begin();
  matrix.setGrayscaleBits(3);
  matrix.clear();
}

void loop() {}