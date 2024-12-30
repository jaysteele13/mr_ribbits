#include <avr/wdt.h>
#include <stdio.h>
#include <string.h>
#include "driveMotor.h"


// Begin declaring functions to be functionised

// Initialise Pins
void MotorDriver::MotorDriverInit(void) 
{
  pinMode(PIN_Motor_PWMA, OUTPUT); // speed on right
  pinMode(PIN_Motor_PWMB, OUTPUT); // speed on left
  pinMode(PIN_Motor_AIN_1, OUTPUT); // direction right
  pinMode(PIN_Motor_BIN_1, OUTPUT); // direction left
  pinMode(PIN_Motor_STBY, OUTPUT); // enable servos?
}

// Declare function to activate pins to control servos

void MotorDriver::MotorDriverControl(boolean direction_A, uint8_t speed_A, boolean direction_B, uint8_t speed_B, boolean enableServo)
{
  // grab public servo enable variable
  if(enableServo == enable_servo) { // do if servos are enabled
    digitalWrite(PIN_Motor_STBY, HIGH);

    // A ... Right Servos
    switch(direction_A) 
    {
      case direction_forw:
        digitalWrite(PIN_Motor_AIN_1, HIGH);
        analogWrite(PIN_Motor_PWMA, speed_A);
        break;
      case direction_back:
        digitalWrite(PIN_Motor_AIN_1, LOW);
        analogWrite(PIN_Motor_PWMA, speed_A);
        break;
      case direction_void:
        analogWrite(PIN_Motor_PWMA, 0);
        digitalWrite(PIN_Motor_STBY, LOW);
        break;
      default:
        analogWrite(PIN_Motor_PWMA, 0);
        digitalWrite(PIN_Motor_STBY, LOW);
        break;
      }

      // B ... Left Servos
      switch (direction_B)
      {
      case direction_forw:
        digitalWrite(PIN_Motor_BIN_1, HIGH);

        analogWrite(PIN_Motor_PWMB, speed_B);
        break;
      case direction_back:
        digitalWrite(PIN_Motor_BIN_1, LOW);
        analogWrite(PIN_Motor_PWMB, speed_B);
        break;
      case direction_void:
        analogWrite(PIN_Motor_PWMB, 0);
        digitalWrite(PIN_Motor_STBY, LOW);
        break;
      default:
        analogWrite(PIN_Motor_PWMB, 0);
        digitalWrite(PIN_Motor_STBY, LOW);
        break;
      }
    }
    else // turn of motor
    {
      digitalWrite(PIN_Motor_STBY, LOW);
      return;
    }

  }

  void MotorDriver::Stop()
  {
    digitalWrite(PIN_Motor_STBY, LOW);
  }