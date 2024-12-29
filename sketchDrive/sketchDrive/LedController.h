#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Arduino.h>
#include <avr/wdt.h>
#include "FastLED.h"

class LedController 
{
  public:
    LedControllerInit(uint8_t set_Brightness = 50);
    SetAndEnableRGB(uint16_t Duration = 100, uint8_t Traversal_Number = 2, CRGB colour = CRGB::Red);
  private:
    #define PIN_RBGLED 4
    #define NUM_LEDS 1
  
  public:
    CRGB leds[NUM_LEDS];
};

#endif