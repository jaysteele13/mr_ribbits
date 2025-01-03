#ifndef SWITCHCONTROLLER_H
#define SWITCHCONTROLLER_H

#include <Arduino.h>

class SwitchController {
public:
    SwitchControllerInit(void);
    bool isSwitchActive();
    bool isButtonActive();
private:
    #define switchPin 2
    #define buttonPin 1
};

#endif
