#include "driveMotor.h"
#include "UltraSonicController.h"
#include "LedController.h"
#include "MiniServoController.h"

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
extern LedController ledController;
extern MiniServoController miniServoController;

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

// static boolean biggerORSmaller(uint8_t value, uint8_t biggest) //f(x)
// {
//   if (0 <= value && value <= biggest)
//     return true;
//   else
//     return false;
// }


// static int16_t ReturnSonicDistance(uint8_t threshold) {
//     uint16_t distance = 0;
//     ultraSonicController.UltraSonicGetReading(&distance);
    
    
//     // if bot is too close to soemthing
//     if(biggerORSmaller(distance, threshold))
//     {
      
//       // force led blink
//       ledController.LedBlink();

//       // log closeness
//       // Serial.print("warning your now past this threshold: ");
//       // Serial.println(threshold);
//       // Serial.print("distance being: ");
//       // Serial.println(distance);

//       // shake robot head
//       miniServoController.SetAngle(65);
//       delay(100);
//       miniServoController.SetAngle(115);
//       delay(100);
  
//     }
//     else
//     {
//       miniServoController.SetAngle(90);
//     }

//     // reset orrientation
//     delay(100);
    
//     miniServoController.Update();

//     return distance;
// }

