/***************************************************
  This version of touchscreen painting is setup to work with PJRC touch
  screen(http://pjrc.com/store/display_ST77XX_touch.html), which uses a 
  different touch controller thatn the Adafruit LI9341 Shield: 
  
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

// Optional Settings. 
// To use Kurt's Flexiboard uncomment the line below:
//#define KURTS_FLEXI

//****************************************************************************
// Touch options, Defaults to SPI
//****************************************************************************
// uncomment to try Touch on SPI1 pins
//#define TOUCH_SPI1
// FlexSPI on SPI1 pins
//#define TOUCH_FLEXSPI


//****************************************************************************
// Includes
//****************************************************************************
#include <Arduino_RouterBridge.h>

#include <XPT2046_Touchscreen.h>
#include <ST77XX_zephyr.h>


//****************************************************************************
// This is calibration data for the raw touch data to the screen coordinates
//****************************************************************************
// Warning, These are
#define TS_MINX 270
#define TS_MINY 370
#define TS_MAXX 3800
#define TS_MAXY 3700

//****************************************************************************
// Settings and objects
//****************************************************************************
// *************** Change to your Pin numbers ***************
#define TFT_DC 4
#define TFT_CS 2
#define TFT_RST 3

// If using SPI1 you can optionally setup to use other MISO pin on T4.1
#define TOUCH_CS 5
#define TOUCH_TIRQ 6

XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_TIRQ);
ST7796_zephyr tft = ST7796_zephyr(&SPI, TFT_CS, TFT_DC, TFT_RST);

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;

//****************************************************************************
// Setup
//****************************************************************************
void setup(void) {
  while (!Serial && (millis() <= 1000));

  Monitor.begin();
  Monitor.println(F("Touch Paint!"));

  tft.init(320, 480);
  // Or by default use SPI
  if (!ts.begin()) {
    Monitor.println("Couldn't start touchscreen controller");
    while (1);
  }
  Monitor.println("Touchscreen started");

  tft.fillScreen(ST77XX_BLACK);

  // make the color selection boxes
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ST77XX_RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ST77XX_YELLOW);
  tft.fillRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, ST77XX_GREEN);
  tft.fillRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, ST77XX_CYAN);
  tft.fillRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, ST77XX_BLUE);
  tft.fillRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, ST77XX_MAGENTA);

  // select the current color 'red'
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ST77XX_WHITE);
  currentcolor = ST77XX_RED;
}


void loop()
{
  // See if there's any  touch data for us
  if (ts.bufferEmpty()) {
    return;
  }

  // You can also wait for a touch
  if (! ts.touched()) {
    return;
  }

  // Retrieve a point
  TS_Point p = ts.getPoint();

  // p is in ST77XX_t3 setOrientation 1 settings. so we need to map x and y differently.
/*
  Monitor.print("X = "); Monitor.print(p.x);
  Monitor.print("\tY = "); Monitor.print(p.y);
  Monitor.print("\tPressure = "); Monitor.print(p.z);
*/
  // Scale from ~0->4000 to tft.width using the calibration #'s
#ifdef SCREEN_ORIENTATION_1
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
#else
  
  uint16_t px = map(p.y, TS_MINY, TS_MAXY, 0, tft.width());
  p.y = map(p.x, TS_MAXX, TS_MINX, 0, tft.height());
  p.x = px;
#endif
/*
    Monitor.print(" ("); Monitor.print(p.x);
    Monitor.print(", "); Monitor.print(p.y);
    Monitor.println(")");
*/
  if (p.y < BOXSIZE) {
    oldcolor = currentcolor;

    if (p.x < BOXSIZE) {
      currentcolor = ST77XX_RED;
      tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ST77XX_WHITE);
    } else if (p.x < BOXSIZE * 2) {
      currentcolor = ST77XX_YELLOW;
      tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ST77XX_WHITE);
    } else if (p.x < BOXSIZE * 3) {
      currentcolor = ST77XX_GREEN;
      tft.drawRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, ST77XX_WHITE);
    } else if (p.x < BOXSIZE * 4) {
      currentcolor = ST77XX_CYAN;
      tft.drawRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, ST77XX_WHITE);
    } else if (p.x < BOXSIZE * 5) {
      currentcolor = ST77XX_BLUE;
      tft.drawRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, ST77XX_WHITE);
    } else if (p.x < BOXSIZE * 6) {
      currentcolor = ST77XX_MAGENTA;
      tft.drawRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, ST77XX_WHITE);
    }

    if (oldcolor != currentcolor) {
      if (oldcolor == ST77XX_RED)
        tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ST77XX_RED);
      if (oldcolor == ST77XX_YELLOW)
        tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ST77XX_YELLOW);
      if (oldcolor == ST77XX_GREEN)
        tft.fillRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, ST77XX_GREEN);
      if (oldcolor == ST77XX_CYAN)
        tft.fillRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, ST77XX_CYAN);
      if (oldcolor == ST77XX_BLUE)
        tft.fillRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, ST77XX_BLUE);
      if (oldcolor == ST77XX_MAGENTA)
        tft.fillRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, ST77XX_MAGENTA);
    }
  }
  if (((p.y - PENRADIUS) > BOXSIZE) && ((p.y + PENRADIUS) < tft.height())) {
    tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
  }
}
