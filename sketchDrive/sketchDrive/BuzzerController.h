#ifndef BUZZERCONTROLLER_H
#define BUZZERCONTROLLER_H

#include <Arduino.h>
#include <avr/wdt.h>

class BuzzerController 
{
  public:
    BuzzerControllerInit(void);
    PlayTheme(void);
  private:
    #define PIN_OUT 11
  
  public:
    
};

#endif