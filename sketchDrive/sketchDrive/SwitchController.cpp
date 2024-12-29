#include "SwitchController.h"

SwitchController::SwitchControllerInit(void) {
    pinMode(switchPin, INPUT);
}

bool SwitchController::isSwitchActive() {
    return digitalRead(switchPin) == HIGH;
}
