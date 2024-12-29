#include "crgb.h"
#include "LedController.h"



LedController::LedControllerInit(uint8_t set_Brightness) {
    FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, 1);
    FastLED.setBrightness(set_Brightness);
}


LedController::SetAndEnableRGB(CRGB colour, uint16_t Duration, uint8_t Traversal_Number)
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

LedController::LedBlink(uint16_t Duration, uint8_t Blink_Count)
{
    for (int i = 0; i < Blink_Count; i++)
    {
        // Turn LEDs ON (keep the current color)
        FastLED.show();
        delay(Duration);

        // Turn LEDs OFF
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
        delay(Duration);

        // Restore previous color (stored in the LED array)
        FastLED.show();
    }
}