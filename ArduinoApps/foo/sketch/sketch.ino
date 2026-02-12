#include <Arduino_RouterBridge.h>
#include <Modulino.h>

ModulinoThermo thermo;

void setRGB(String rgb_str)
{
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Bridge.begin();
    Modulino.begin(Wire1);
    thermo.begin();
    Bridge.provide("setRGB", setRGB);
}

void loop()
{
    Monitor.println("Loop.");
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);       
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);       
}