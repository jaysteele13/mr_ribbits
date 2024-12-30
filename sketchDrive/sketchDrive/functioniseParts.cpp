#include "driveMotor.h"
#include "UltraSonicController.h"
#include "LedController.h"
#include "MiniServoController.h"

// temp vars
static unsigned long previousLedTime = 0;  // Store last LED toggle time
static unsigned long previousServoTime = 0; // Store last servo move time

static const unsigned long ledInterval = 500;  // Interval for LED blink (ms)
static const unsigned long servoInterval = 200; // Interval for servo move (ms)

static bool ledState = false;             // Track LED state
static int servoPosition = 65;            // Current servo position
static bool servoDirection = true;        // Direction of servo movement

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

static boolean biggerORSmaller(uint8_t value, uint8_t biggest) //f(x)
{
  if (0 <= value && value <= biggest)
    return true;
  else
    return false;
}

// temp handle asynchronous
static void handleLedBlink() {
  unsigned long currentTime = millis();
  if (currentTime - previousLedTime >= ledInterval) {
    // Toggle LED state
    ledState = !ledState;
    digitalWrite(4, ledState);
    previousLedTime = currentTime; // Update the last blink time
  }
}

static void handleServoShake() {
  unsigned long currentTime = millis();
  if (currentTime - previousServoTime >= servoInterval) {
    // Move servo between 65 and 115 degrees
    if (servoDirection) {
      servoPosition += 10;
      if (servoPosition >= 115) {
        servoDirection = false; // Change direction
      }
    } else {
      servoPosition -= 10;
      if (servoPosition <= 65) {
        servoDirection = true; // Change direction
      }
    }
    miniServoController.SetAngle(servoDirection);
    previousServoTime = currentTime; // Update the last servo move time
  }
}

static int16_t ReturnSonicDistance(uint8_t threshold) {
    uint16_t distance = 0;
    ultraSonicController.UltraSonicGetReading(&distance);
    
    // if bot is too close to soemthing
    if(biggerORSmaller(distance, threshold))
    {
      // force led blink
      // ledController.LedBlink();
      handleLedBlink();

      // log closeness
      Serial.print("warning your now past this threshold: ");
      Serial.println(threshold);
      Serial.print("distance being: ");
      Serial.println(distance);

      // shake robot head
      handleServoShake();
      // miniServoController.SetAngle(65);
      // delay(100);
      // miniServoController.SetAngle(115);
  
    }

    // reset orrientation
    delay(100);
    miniServoController.SetAngle(90);

    return distance;
}

