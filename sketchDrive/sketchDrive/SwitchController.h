#ifndef SWITCHCONTROLLER_H
#define SWITCHCONTROLLER_H

#include <Arduino.h>

class SwitchController {
public:
    SwitchControllerInit(void);
    bool isSwitchActive();
private:
    #define switchPin 2
};

#endif
