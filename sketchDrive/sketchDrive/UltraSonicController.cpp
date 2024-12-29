#include "UltraSonicController.h"

UltraSonicController::UltraSonicControllerInit(void)
{
  pinMode(ECHO_PIN, INPUT); //Ultrasonic module initialization
  pinMode(TRIG_PIN, OUTPUT);
}
UltraSonicController::UltraSonicGetReading(uint16_t *ULTRASONIC_Get /*out*/)
{
  // get every 10 milliseconds?
  unsigned int tempda_x = 0;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(500);
  digitalWrite(TRIG_PIN, LOW);
  tempda_x = ((unsigned int)pulseIn(ECHO_PIN, HIGH) / 58);
  *ULTRASONIC_Get = tempda_x;
  // sonar.ping() / US_ROUNDTRIP_CM; // Send ping, get ping time in microseconds (uS).
}