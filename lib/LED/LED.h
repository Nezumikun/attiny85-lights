#ifndef NEZUMIKUN_LED
#define NEZUMIKUN_LED

#include <Arduino.h>

namespace Nezumikun {
  class LED {
    public:
      enum class LedMode {
        LED_ON,
        LED_OFF,
        LED_BLINK,
        LED_FLASH
      };
    private:
      int               pinNumber;
      LedMode           mode = LedMode::LED_OFF;
      unsigned int      interval = 500;
      unsigned long     time = 0;
      int               state = LOW;
    public:
      explicit LED(int pin);
      void on();
      void off();
      void blink(unsigned int interval);
      void flash(unsigned int interval);
      void touch();
      void touch(unsigned long now);
  };
}

#endif