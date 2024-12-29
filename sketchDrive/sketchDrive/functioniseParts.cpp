#include "driveMotor.h"
#include "UltraSonicController.h"

enum DirectionControl
{
  Forward,
  Backward,
  Left,
  Right,
  LeftForward,
  LeftBackward,
  RightForward,
  RightBackward,
  stop_it
};

struct Application
{
  DirectionControl directionControl;
};

//extern Application application
extern MotorDriver motorDriver;
extern UltraSonicController ultraSonicController;

static void ControlBot(DirectionControl direction, uint8_t _speed) 
{
  static uint8_t directionRecord = 0;
  uint8_t lowerLimit, upperLimit;
  uint8_t speed = _speed;

  switch(direction) 
  {
    case Forward:
      motorDriver.MotorDriverControl(direction_forw, speed, direction_forw, speed);
      break;
    case Backward:
      motorDriver.MotorDriverControl(direction_back, speed, direction_back, speed);
      break;
    case Left:
      motorDriver.MotorDriverControl(direction_forw, speed, direction_back, speed);
      break;
    case Right:
      motorDriver.MotorDriverControl(direction_back, speed, direction_forw, speed);
      break;
    case LeftForward:
      motorDriver.MotorDriverControl(direction_forw, speed, direction_forw, speed / 2);
      break;
    case LeftBackward:
      motorDriver.MotorDriverControl(direction_back, speed, direction_back, speed / 2);
      break;
    case RightForward:
      motorDriver.MotorDriverControl(direction_forw, speed / 2, direction_forw, speed);
      break;
    case RightBackward:
      motorDriver.MotorDriverControl(direction_back, speed / 2, direction_back, speed);
      break;
    case stop_it:
      motorDriver.MotorDriverControl(direction_void, 0, direction_void, 0);
      break;
    default:
      break;
  }
}

static boolean biggerORSmaller(uint8_t value, uint8_t smallest, uint8_t biggest) //f(x)
{
  if (smallest <= value && value <= biggest)
    return true;
  else
    return false;
}

static int16_t ReturnSonicDistance() {
    uint16_t distance = 0;
    ultraSonicController.UltraSonicGetReading(&distance);
    //Serial.print("Here is distance: ");
    //Serial.println(distance);
    return distance;
}

