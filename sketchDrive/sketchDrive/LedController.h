#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Arduino.h>
#include <avr/wdt.h>
#include <FastLED.h>

class LedController 
{
  public:
    LedControllerInit(uint8_t set_Brightness = 30);
    SetAndEnableRGB(CRGB colour = CRGB::Red, uint16_t Duration = 100, uint8_t Traversal_Number = 2);
    LedBlink(uint16_t Duration = 100, uint8_t Blink_Count = 2);
  private:
    #define PIN_RBGLED 4
    #define NUM_LEDS 1
  
  public:
    CRGB leds[NUM_LEDS];
    CRGB current_colour;
};

#endif