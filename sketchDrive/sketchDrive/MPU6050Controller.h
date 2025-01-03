#ifndef MPU6050CONTROLLER_H
#define MPU6050CONTROLLER_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

class MPU6050Controller {
  public:
    Init(void);
    int GetTemperature();
  private:
    Adafruit_MPU6050 mpu;


};

#endif