#ifndef NEZUMIKUN_LED_MANAGER
#define NEZUMIKUN_LED_MANAGER

#include "Arduino.h"
#include "FastLED.h"

#define NEZUMIKUN_LED_MANAGER_EFFECTS_NUMBER 7

namespace Nezumikun {
  class LedManager {
    private:
      uint8_t ledsNumber;
      CRGB* leds;
      uint8_t timeInterval;
      bool demoMode;
      unsigned long prevTime = 0;
      uint8_t currentPatternNumber = 0; // Index number of which pattern is current
      uint8_t hue = 0; // rotating "base color" used by many of the patterns
      uint16_t x = 0;
      uint16_t y = 0;
      uint16_t z = 0;
      void effectRainbow();
      void effectRainbowWithGlitter();
      void effectConfetti();
      void effectSinelon();
      void effectBpm();
      void effectJuggle();
      void effectPerlinNoise();
    public:
      LedManager(CRGB* leds, uint8_t ledsNumber, uint8_t framesPerSecond, bool demoMode = true);
      void begin();
      void loop(unsigned long now);
      bool isDemoMode();
      void setDemoMode(bool demoMode);
      void nextPattern(bool autoChange);
  };
}

#endif