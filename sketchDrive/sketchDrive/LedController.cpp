#include "LedController.h"
#include "crgb.h"

LedController::LedControllerInit(uint8_t set_Brightness) {
  FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, 1);
  FastLED.setBrightness(set_Brightness);
}

LedController::SetAndEnableRGB(CRGB colour, uint16_t Duration,
                               uint8_t Traversal_Number) {
  current_colour = colour;  // Store the color to current_colour

  if (NUM_LEDS < Traversal_Number) {
    Traversal_Number = NUM_LEDS;
  }

  for (int Number = 0; Number < Traversal_Number; Number++) {
    leds[Number] = current_colour;  // Set the LED to the specified color
  }

  FastLED.show();  // Update LEDs after setting them
}

LedController::LedBlink(uint16_t Duration, uint8_t Blink_Count) {
  CRGB previous_colour =
      current_colour;  // Save the current color before blinking

  for (int i = 0; i < Blink_Count; i++) {
    // Turn LEDs ON (keep the current color)
    for (int j = 0; j < NUM_LEDS; j++) {
      leds[j] =
          previous_colour;  // Ensure the LEDs are set to the previous color
    }
    FastLED.show();  // Update LEDs to show the current color
    delay(Duration);

    // Turn LEDs OFF
    fill_solid(leds, NUM_LEDS, CRGB::Black);  // Set LEDs to black
    FastLED.show();                           // Update LEDs to turn them off
    delay(Duration);

    // Restore previous color (stored in the LED array)
    for (int j = 0; j < NUM_LEDS; j++) {
      leds[j] = previous_colour;  // Set the LEDs back to the previous color
    }
    FastLED.show();  // Update LEDs to restore the color
  }
}
