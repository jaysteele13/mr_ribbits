#include "crgb.h"
#include "LedController.h"



LedController::LedControllerInit(uint8_t set_Brightness) {
    FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, 1);
    FastLED.setBrightness(set_Brightness);
}


LedController::SetAndEnableRGB(uint16_t Duration, uint8_t Traversal_Number, CRGB colour)
{
  if (NUM_LEDS < Traversal_Number)
  {
    Traversal_Number = NUM_LEDS;
  }
  for (int Number = 0; Number < Traversal_Number; Number++)
  {
    leds[Number] = colour;
    FastLED.show();
    delay(Duration);
  }
}