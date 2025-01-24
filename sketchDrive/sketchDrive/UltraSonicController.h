#ifndef ULTRASONICCONTROLLER_H
#define ULTRASONICCONTROLLER_H

#include <Arduino.h>

class UltraSonicController 
{
  public:
    UltraSonicControllerInit(void);
    UltraSonicGetReading(uint16_t *ULTRASONIC_Get /*out*/);
  private:
    #define TRIG_PIN 13      // Arduino pin tied to trigger pin on the ultrasonic sensor.
    #define ECHO_PIN 12      // Arduino pin tied to echo pin on the ultrasonic sensor.
    #define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
};

#endif