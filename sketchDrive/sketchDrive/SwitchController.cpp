#include "Arduino.h"
#include "SwitchController.h"

SwitchController::SwitchControllerInit(void) {
    pinMode(switchPin, INPUT);
    pinMode(buttonPin, INPUT_PULLUP);
}

bool SwitchController::isSwitchActive() 
{
  return digitalRead(switchPin) == HIGH;
}

bool SwitchController::isButtonActive()
{
  return digitalRead(buttonPin) == HIGH;
}
