#include "HardwareSerial.h"
#include "Autopilot.h"

AutoPilot::Init() 
{
  // Do I need this?
}

AutoPilot::Move(DirectionControl direction, uint8_t _speed) 
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






AutoPilot::SetAutoPilot(bool toggle)
{
  isActive = toggle;
}

AutoPilot::Roam()
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
    delay(15);
    state = RobotState::Moving;
    //Serial.println("should move forward");
    //Move(DirectionControl::Forward, defaultSpeed);
    delay(500);
    AutoPilot::Stop();
    // PivotByEdge();
    PivotBySensor();
    
    // for tomorrow:
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
          Move(DirectionControl::RightBackward, defaultSpeed);
          Serial.print("Right backward by edge - left!");
          delay(2000);
          break;
        case EdgeDetectionController::Sensor::Middle:
          Move(DirectionControl::Backward, defaultSpeed);
          Serial.print("backward by middle wrong!");
          delay(2000);
          Move(DirectionControl::Right, defaultSpeed);
          break;
        case EdgeDetectionController::Sensor::Right:
          Serial.print("Left backward by edge - right!");
          Move(DirectionControl::LeftBackward, defaultSpeed);
          delay(2000);
          break;
        default:
          Serial.print("Something went wrong!");
      }
      Reverse180();
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
  Serial.print("detecting obstacle");
}

// test
// AutoPilot::PivotBySensor(uint8_t threshold) {
//     miniServoController.Update();
//     sonic_distance = miniServoController.GetAngle();
//     Serial.println("turn 150 degree");
//     miniServoController.SetAngle(150);
    
//     delay(2000);

//     miniServoController.SetAngle(180);

//     bool checkLeft = false;
//     bool checkMiddle = false;

//     ledController.LedBlink();
//     miniServoController.SetAngle(90);
//     Serial.println("go 90 degrees");
//     // miniServoController.Update();
//     delay(2000); 
//     miniServoController.SetAngle(120);
//     Serial.println("go 120 degrees");
//       delay(2000); 
//     //miniServoController.Update();
//     delay(1000); 
//     miniServoController.SetAngle(150);
//     // miniServoController.Update();
//     // delay(1000); 
//     // // miniServoController.Update();
//     // // check right hand side for blockages
//     // delay(1000);

//     // // reset default
//     // miniServoController.SetAngle(90);
//     // // miniServoController.Update(); 
//     // delay(15); 

//     // if(checkLeft) 
//     // {
//     //     // check left hand side for blockages
//     //   for(int i = 90; i > 0; i -= 15) 
//     //   {
//     //     if(DetectObstacle(threshold))
//     //     {
//     //             // force led blink
//     //       ledController.LedBlink();
//     //       miniServoController.SetAngle(i);
//     //       delay(1000);
//     //       miniServoController.Update(); 
//     //     }
//     //     else {
//     //       checkMiddle = true;
//     //       Move(DirectionControl::Left,  defaultSpeed);
//     //       delay(500);
//     //       break;
//     //     }
        
//     //   }

//     // }

    // // reset defaults
    // miniServoController.SetAngle(90);
    // miniServoController.Update(); 
    // delay(15); 

    // if haven't yet moved
    // reverse 180
    // if(checkMiddle) 
    // {
    //   Reverse180();
    // }
    
// }


  // check sonic sensor real
AutoPilot::PivotBySensor(uint8_t threshold) {
    miniServoController.Update(); 
  
    sonic_distance = miniServoController.GetAngle();
    miniServoController.SetAngle(90);

    bool checkLeft = false;
    bool checkMiddle = false;

    if(DetectObstacle(threshold))
    {
      
    }
    // check right hand side for blockages
    for(int i = 90; i < 180; i += 30) 
    {
      if(DetectObstacle(threshold))
      {
              // force led blink
        ledController.LedBlink();
        Serial.println("check right");
        miniServoController.SetAngle(i);
        miniServoController.Update(); 
        delay(1000); 
        
      }
      else {
        //miniServoController.Update();
        checkLeft = true;
        Move(DirectionControl::Right,  defaultSpeed);
        delay(100);
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
          Serial.println("check right");
          ledController.LedBlink();
           
          miniServoController.SetAngle(i);
          miniServoController.Update();
          delay(1000);
          
        }
        else {
          checkMiddle = true;
          Move(DirectionControl::Left,  defaultSpeed);
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
  Serial.println("Reverse 180");
  Move(DirectionControl::Right, 100);
  delay(1000);
}

AutoPilot::Stop() 
{
  motorDriver.Stop();
  state = RobotState::Stopped;
}

