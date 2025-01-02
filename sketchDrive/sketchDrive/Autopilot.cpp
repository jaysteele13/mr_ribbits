#include "HardwareSerial.h"
#include "Arduino.h"
#include "Autopilot.h"

AutoPilot::Init()
{
  edgeDetectionController.Init();
  motorDriver.MotorDriverInit();
  buzzerController.BuzzerControllerInit();
  switchController.SwitchControllerInit();
  ledController.LedControllerInit();
  ultraSonicController.UltraSonicControllerInit();
  miniServoController.MiniServoControllerInit();
}

AutoPilot::Move(DirectionControl direction, uint8_t _speed) {
  static uint8_t directionRecord = 0;
  uint8_t lowerLimit, upperLimit;
  uint8_t speed = _speed;

  switch (direction) {
    case Forward:
      motorDriver.MotorDriverControl(direction_forw, speed, direction_forw,
                                     speed);
      break;
    case Backward:
      motorDriver.MotorDriverControl(direction_back, speed, direction_back,
                                     speed);
      break;
    case Left:
      motorDriver.MotorDriverControl(direction_forw, speed, direction_back,
                                     speed);
      break;
    case Right:
      motorDriver.MotorDriverControl(direction_back, speed, direction_forw,
                                     speed);
      break;
    case LeftForward:
      motorDriver.MotorDriverControl(direction_forw, speed, direction_forw,
                                     speed / 2);
      break;
    case LeftBackward:
      motorDriver.MotorDriverControl(direction_back, speed, direction_back,
                                     speed / 2);
      break;
    case RightForward:
      motorDriver.MotorDriverControl(direction_forw, speed / 2, direction_forw,
                                     speed);
      break;
    case RightBackward:
      motorDriver.MotorDriverControl(direction_back, speed / 2, direction_back,
                                     speed);
      break;
    case stop_it:
      motorDriver.MotorDriverControl(direction_void, 0, direction_void, 0);
      break;
    default:
      break;
  }
}

AutoPilot::SetAutoPilot(bool toggle) { isActive = toggle; }

AutoPilot::Roam() {
  if (isActive && IsGrounded()) {
    // default go straight
    ledController.SetAndEnableRGB(CRGB::DarkGreen);
    delay(15);
    state = RobotState::Moving;
    // Serial.println("should move forward");
    Move(DirectionControl::Forward, defaultSpeed);
    delay(500);
    AutoPilot::Stop();
    PivotByEdge();
    PivotBySensor();
    StopBySwitch();
  }
  else if(!IsGrounded())
  {
    if(grounded_song_flag) 
    {
      Stop();
      isActive = false; // Disable AutoPilot
      buzzerController.PlayTetris();
      grounded_song_flag = false;
      ledController.SetAndEnableRGB();
      delay(10);
    }
  }
  else {
    ledController.SetAndEnableRGB();
  }
  
}

AutoPilot::PivotByEdge() {
  // Now check if sensors are ok
  sensorProxy = edgeDetectionController.ScanForEdges();
  if (sensorProxy != EdgeDetectionController::Sensor::None) {
    switch (sensorProxy) {
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

AutoPilot::DetectObstacle(uint8_t threshold) {
  uint16_t distance = 0;
  ultraSonicController.UltraSonicGetReading(&distance);

  if (0 <= distance && distance <= threshold)
    return true;
  else
    return false;
  Serial.print("detecting obstacle");
}

// check sonic sensor real
AutoPilot::PivotBySensor(uint8_t threshold) 
{
  bool checkLeft = false;
  bool checkMiddle = false;

  if (DetectObstacle(threshold)) {
    // check right hand side for blockages
    for (int i = 120; i <= 180; i += 30) {
      ledController.LedBlink();
      miniServoController.SetAngle(i);
      delay(100);

      if (!DetectObstacle(threshold)) {
        Move(DirectionControl::Right, defaultSpeed * 2);
        delay(1000);
        break;
      }

      if (i == 180) {
        delay(100);  // delay to update
        checkLeft = true;
      }
    }
    miniServoController.SetAngle(90);
    delay(15);

    if (checkLeft) {
      // check left hand side for blockages
      for (int i = 90; i >= 0; i -= 30) {
        // force led blink
        ledController.LedBlink();
        miniServoController.SetAngle(i);
        delay(100);

        if (!DetectObstacle(threshold)) {
          Move(DirectionControl::Left, defaultSpeed * 2);
          delay(1000);

          break;
        }

        if (i == 180) {
          checkMiddle = true;
        }
      }
    }
    if (checkMiddle) {
      Reverse180();
    }
  }
  miniServoController.SetAngle(90);
}

AutoPilot::Reverse180() {
  Serial.println("Reverse 180");
  Move(DirectionControl::Right, 100);
  delay(1000);
}

AutoPilot::Stop() {
  motorDriver.Stop();
  state = RobotState::Stopped;
}

AutoPilot::StopBySwitch()
{
  if(!switchController.isSwitchActive()) 
  {
    isActive = false;
    Stop();
    // Stand By Sound
    buzzerController.PlayTheLick();
    delay(500);
  }
  
}

AutoPilot::SetGroundedMusicFlag(bool toggle)
{
  grounded_song_flag = toggle;
}

bool AutoPilot::IsGrounded()
{
    delay(50); // stabolitly;
    static unsigned long startTime = 0;
    const unsigned long debounceDuration = 3000; // 2 seconds in milliseconds

    // Read sensor values
    int leftValue = analogRead(LEFT_SENSOR_PIN);
    int middleValue = analogRead(MIDDLE_SENSOR_PIN);
    int rightValue = analogRead(RIGHT_SENSOR_PIN);

    // godly lambda function resolves this!
    auto checkPersistence = [](int value, int threshold, unsigned long& startTime) -> bool {
        if (value > threshold) {
            if (startTime == 0) {
                startTime = millis(); // Start timing when threshold is first exceeded
            }
            // Check if the condition has persisted long enough
            if (millis() - startTime >= debounceDuration) {
                return false;
            }
        } else {
            // Reset the timer if the value drops below the threshold
            startTime = 0;
        }
        return true;
    };

    bool ground = checkPersistence(leftValue, GROUNDED_THRESHOLD, startTime);

    if(ground) 
    {
      grounded_song_flag = true;
    }

    return ground;

    
}

