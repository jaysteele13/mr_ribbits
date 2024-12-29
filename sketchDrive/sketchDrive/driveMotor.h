#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <Arduino.h>

// Define Motor class
class MotorDriver {
  public:
    void MotorDriverInit(void);
    #if _Debug_MotorDriver
      void MotorDriverDebug(void);
    #endif
      void MotorDriverControl(boolean direction_A, uint8_t speed_A, // right hand side
      boolean direction_B, uint8_t speed_B, boolean enableServo = true);
    
  private:
    #define PIN_Motor_PWMA 5
    #define PIN_Motor_PWMB 6
    #define PIN_Motor_BIN_1 8
    #define PIN_Motor_AIN_1 7
    #define PIN_Motor_STBY 3

  public:
    #define speed_Max 255
    #define direction_forw true
    #define direction_back false
    #define direction_void 3

    #define Duration_enable true
    #define Duration_disable false
    #define enable_servo true
    #define disable_servo false
  };

  #endif