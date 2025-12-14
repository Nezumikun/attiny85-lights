#ifndef NEZUMIKUN_LED_MANAGER
#define NEZUMIKUN_LED_MANAGER

#include "Arduino.h"
#include "FastLED.h"

#define NEZUMIKUN_LED_MANAGER_EFFECTS_NUMBER 6

#ifdef BOARD_ATTINY85
#define NANO_PRINT(X) // Serail.print(X)
#define NANO_PRINTLN(X) // Serail.println(X)
#else
#define NANO_PRINT(X) Serial.print(X)
#define NANO_PRINTLN(X) Serial.println(X)
#endif

namespace Nezumikun {
  typedef void (*ptrAllModesCallBack)();
  class LedManager {
    private:
      uint8_t ledsNumber;
      CRGB* leds;
      uint8_t timeInterval;
      bool demoMode;
      unsigned long prevTime = 0;
      uint8_t currentEffectNumber = 0; // Index number of which pattern is current
      uint8_t hue = 0; // rotating "base color" used by many of the patterns
      uint16_t x = 0;
      uint16_t y = 0;
      uint8_t subMode = 0;
      void effectRainbow();
      void effectConfetti();
      void effectSinelon();
      void effectBpm();
      void effectJuggle();
      void effectPerlinNoise();
      void nextPalette();
      ptrAllModesCallBack callbackAllModes = NULL;
      bool isShowAllModesInDemo = false;
    public:
      LedManager(CRGB* leds, uint8_t ledsNumber, uint8_t framesPerSecond, bool demoMode = true);
      void begin();
      void loop(unsigned long now);
      bool isDemoMode();
      void setDemoMode(bool demoMode);
      void nextEffect();
      void setAllModesCallback(ptrAllModesCallBack callback);
  };
}

#endif