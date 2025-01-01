#include "Autopilot.h"

AutoPilot::Init() 
{
  // Do I need this?
}

AutoPilot::Roam(int duration)
{
  /* When auto pilot roaming I must be checking many things
  - Sensor to see whats in fron
  - edgeDetection to not fall off things
  - perhaps current rotation
  - check every 500ms?
  */
  if(isActive)
  {
    // default go straight
    state = RobotState::Moving;
    ControlBot(DirectionControl::Forward, 100);
    delay(500);
    AutoPilot::Stop();

    // Now check if sensors are ok
    sensorProxy = edgeDetectionController.ScanForEdges();
    if(sensorProxy != EdgeDetectionController::Sensor::None)
    {
      switch(sensorProxy) 
      {
        case EdgeDetectionController::Sensor::Left:
          ControlBot(DirectionControl::RightBackward, 50);
          delay(500);
          break;
        case EdgeDetectionController::Sensor::Middle:
          ControlBot(DirectionControl::Backward, 100);
          delay(500);
          ControlBot(DirectionControl::Right, 150);
          break;
        case EdgeDetectionController::Sensor::Right:
          ControlBot(DirectionControl::LeftBackward, 50);
          delay(500);
          break;
        default:
          Serial.print("Something went wrong!");
      }
    }

  

    // if stopped either turn left or right... slowly and go there
  }
}

boolean biggerORSmaller(uint8_t value, uint8_t biggest) //f(x)
{
  if (0 <= value && value <= biggest)
    return true;
  else
    return false;
}

  // check sonic sensor
int16_t ScanSonicDistance(uint8_t threshold) {
    uint16_t distance = 0;
    ultraSonicController.UltraSonicGetReading(&distance);
    
    // while something is picked up in threshold
    // if bot is too close to soemthing
    while(biggerORSmaller(distance, threshold))
    {
      
      // force led blink
      ledController.LedBlink();
    
      // check right hand side for blockages

      // check left handside for blockages

      // if no scenarios reverse and randomly roam

      // shake robot head
      miniServoController.SetAngle(65);
      delay(100);
      miniServoController.SetAngle(115);
      delay(100);
  
    }

    // reset orrientation
    delay(100);
    
    miniServoController.Update();

    return distance;
}

AutoPilot::Stop() 
{
  motorDriver.Stop();
  state = RobotState::Stopped;
}

