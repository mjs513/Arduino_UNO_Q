
//****************************************************************************
// Touch options, Defaults to SPI
//****************************************************************************
// uncomment to try Touch on SPI1 pins
//#define TOUCH_SPI1
// FlexSPI on SPI1 pins
//#define TOUCH_FLEXSPI

#ifdef ARDUINO_UNO_Q
#include <Arduino_RouterBridge.h>
#define SerialX Monitor
#define TFT_DC 4
#define TFT_CS 2
#define TFT_RST 3

// optional use my version of the ST77xx code
//#define USE_ST77XX_zephyr

#else
#define SerialX Serial
#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST -1
#endif

//****************************************************************************
// Includes
//****************************************************************************

//#include <XPT2046_Touchscreen.h>
#ifdef USE_ST77XX_zephyr
#include <ST77XX_zephyr.h>
#else
#include <Adafruit_GFX.h>
#include <Adafruit_ST7796S.h>
#endif
#include <Wire.h>  // this is needed for FT6206
#include <FT6236G.h>


//****************************************************************************
// Settings and objects
//****************************************************************************
// *************** Change to your Pin numbers ***************

// If using SPI1 you can optionally setup to use other MISO pin on T4.1
#define TOUCH_TIRQ 6
#define ROTATION 1
uint8_t g_touch_rotation = ROTATION;
// The FT6206 uses hardware I2C (SCL/SDA)
FT6236G ctp = FT6236G();

//XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_TIRQ);
#ifdef USE_ST77XX_zephyr
ST7796_zephyr tft = ST7796_zephyr(&SPI, TFT_CS, TFT_DC, TFT_RST);
#else
Adafruit_ST7796S tft(TFT_CS, TFT_DC, TFT_RST);
#endif

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3

// Colors
static const uint16_t paint_colors[] = { ST77XX_RED, ST77XX_YELLOW, ST77XX_GREEN, ST77XX_CYAN, ST77XX_BLUE, ST77XX_MAGENTA };
#define COUNT_PAINT_COLORS (sizeof(paint_colors) / sizeof(paint_colors[0]))
uint8_t current_color_index = 0;


//****************************************************************************
// Setup
//****************************************************************************
void setup(void) {
  while (!Serial && (millis() <= 1000))
    ;

#ifdef ARDUINO_UNO_Q
  SerialX.begin();
#else
  SerialX.begin(115200);
#endif
  SerialX.println(F("Touch Paint!"));

  tft.init(320, 480);
#ifdef USE_ST77XX_zephyr
  tft.invertDisplay(true);
#endif
  // Or by default use SPI
  if (ctp.init(0xff, 0xff, false, 400000) != FT_SUCCESS) {  // pass in 'sensitivity' coefficient
    SerialX.println("Couldn't start FT6236 touchscreen controller");
    while (1)
      ;
  }

  SerialX.println("Touchscreen started");
  SerialX.println("You can enter 0-3 to change rotation");

  setRotation(ROTATION);
  while (SerialX.available()) SerialX.read();
  SerialX.println("SerialX queue cleared");
}

void setRotation(uint8_t rotation) {
  g_touch_rotation = rotation;
  uint32_t start_time = micros();
  tft.setRotation(g_touch_rotation);
  tft.fillScreen(ST77XX_BLACK);

  // make the color selection boxes
  for (uint8_t i = 0; i < COUNT_PAINT_COLORS; i++) {
    tft.fillRect(BOXSIZE * i, 0, BOXSIZE, BOXSIZE, paint_colors[i]);
  }
  // select the current color 'red'
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ST77XX_WHITE);
  uint32_t delta_time = (uint32_t)micros() - start_time;
  SerialX.print("Display time: ");
  SerialX.println(delta_time);
  current_color_index = 0;
}

void convertRawTouchByRotation(uint16_t xRaw, uint16_t yRaw, uint16_t &touch_x, uint16_t &touch_y) {
  int tmp;
  switch (g_touch_rotation) {
    case 0:
      touch_x = xRaw;
      touch_y = yRaw;
      break;
    case 1:
      tmp = xRaw;
      touch_x = yRaw;
      touch_y = tft.height() - tmp;
      break;
    case 2:
      touch_x = tft.width() - xRaw;
      touch_y = tft.height() - yRaw;
      break;
    case 3:
      tmp = xRaw;
      touch_x = tft.width() - yRaw;
      touch_y = tmp;
      break;
  }
}

void loop() {
  // Wait for a touch
  TOUCHINFO ti;
  uint16_t touch_x, touch_y;


  int ch;
  if ((ch = SerialX.available()) > 0) {
    SerialX.print("Serial available:");
    SerialX.println(ch);
    uint8_t new_rototation = 0;
    while (SerialX.available()) {
      ch = SerialX.read();
      //while ((ch = SerialX.read()) != -1) {
      //SerialX.write(ch);
      if ((ch >= '0') && (ch <= '9')) {
        new_rototation = ch - '0';
      }
    }
    setRotation(new_rototation);
    SerialX.print("New rotatation: ");
    SerialX.println(new_rototation);
  }

  if (ctp.getSamples(&ti) != FT_SUCCESS) return;  // something went wrong

  if (ti.count == 0) return;


  // p is in ST77XX_t3 setOrientation 1 settings. so we need to map x and y differently.
  /*
  SerialX.print("X = ");
  SerialX.print(ti.x[0]);
  SerialX.print("\tY = ");
  SerialX.print(ti.y[0]);
  SerialX.print("\tPressure = ");
  SerialX.print(ti.pressure[0]);
  */
  // Scale from ~0->4000 to tft.width using the calibration #'s
  convertRawTouchByRotation(ti.x[0], ti.y[0], touch_x, touch_y);

  /*
  SerialX.print(" (");
  SerialX.print(x);
  SerialX.print(", ");
  SerialX.print(y);
  SerialX.println(")");
  */

  if (touch_y < BOXSIZE) {
    uint8_t new_color_index = touch_x / BOXSIZE;
    if ((new_color_index != current_color_index) && (new_color_index < COUNT_PAINT_COLORS)) {
      // highlight the new touch color
      tft.drawRect(BOXSIZE * new_color_index, 0, BOXSIZE, BOXSIZE, ST77XX_WHITE);
      // unhighlight the previous one.
      tft.drawRect(BOXSIZE * current_color_index, 0, BOXSIZE, BOXSIZE, paint_colors[current_color_index]);
      current_color_index = new_color_index;
    }
  }

  if (((touch_y - PENRADIUS) > BOXSIZE) && ((touch_y + PENRADIUS) < tft.height())) {
    tft.fillCircle(touch_x, touch_y, PENRADIUS, paint_colors[current_color_index]);
  }

  for (uint8_t i = 1; i < ti.count; i++) {
    convertRawTouchByRotation(ti.x[i], ti.y[i], touch_x, touch_y);
    if (((touch_y - PENRADIUS) > BOXSIZE) && ((touch_y + PENRADIUS) < tft.height())) {
      uint8_t color_index = current_color_index + i;
      if (color_index >= COUNT_PAINT_COLORS) color_index -= COUNT_PAINT_COLORS;
      tft.fillCircle(touch_x, touch_y, PENRADIUS, paint_colors[color_index]);
    };
  }
}
