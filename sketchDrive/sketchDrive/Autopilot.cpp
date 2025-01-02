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
    PivotByEdge();
    PivotBySensor(20) 
    
    // for tomorrow:
    // get default config
    // amend to signal buzzer noise if something is picked up?
    // amend so roam has more random movement

    // Coding wise finish other demo things for uni!
    
  }
}

AutoPilot::PivotByEdge()
{
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

}


AutoPilot::DetectObstacle (uint8_t threshold)
{
  uint16_t distance = 0;
  ultraSonicController.UltraSonicGetReading(&distance);

  if (0 <= distance && distance <= threshold)
    return true;
  else
    return false;

}

  // check sonic sensor
AutoPilot::PivotBySensor(uint8_t threshold) {
  
    sonic_distance = miniServoController.GetAngle();
    miniServoController.SetAngle(90);

    bool checkLeft = false;
    bool checkMiddle = false;

    // check right hand side for blockages
    for(int i = 90; i < 180; i += 15) 
    {
      if(DetectObstacle(threshold))
      {
              // force led blink
        ledController.LedBlink();
        miniServoController.SetAngle(i);
        delay(15);
        miniServoController.Update(); 
      }
      else {
        checkLeft = true;
        ControlBot(DirectionControl::Right,  50);
        delay(500);
        break;
      }
      
    }

    // reset default
    miniServoController.SetAngle(90);
    miniServoController.Update(); 
    delay(15); 

    if(checkLeft) 
    {
        // check left hand side for blockages
      for(int i = 90; i > 0; i -= 15) 
      {
        if(DetectObstacle(threshold))
        {
                // force led blink
          ledController.LedBlink();
          miniServoController.SetAngle(i);
          delay(15);
          miniServoController.Update(); 
        }
        else {
          checkMiddle = true;
          ControlBot(DirectionControl::Left,  50);
          delay(500);
          break;
        }
        
      }

    }

    // reset defaults
    miniServoController.SetAngle(90);
    miniServoController.Update(); 
    delay(15); 

    // if haven't yet moved
    // reverse 180
    if(checkMiddle) 
    {
      Reverse180();
    }
    
}

AutoPilot::Reverse180()
{
  ControlBot(DirectionControl::Backward, 200);
  delay(500);
  ControlBot(DirectionControl::Right, 150);
}

AutoPilot::Stop() 
{
  motorDriver.Stop();
  state = RobotState::Stopped;
}

