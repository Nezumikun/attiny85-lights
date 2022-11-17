#include "LED.h"

namespace Nezumikun {

  LED::LED(int pin) {
    this->pinNumber = pin;
    pinMode(this->pinNumber, OUTPUT);
    this->off();
  }

  void LED::on() {
    this->mode = LedMode::LED_ON;
    this->state = HIGH;
    digitalWrite(this->pinNumber, HIGH);
  }

  void LED::off() {
    this->mode = LedMode::LED_OFF;
    this->state = LOW;
    digitalWrite(this->pinNumber, LOW);
  }

  void LED::blink(unsigned int interval) {
    this->mode = LedMode::LED_BLINK;
    this->interval = interval;
    this->state = (this->state == HIGH) ? LOW : HIGH;
    this->time = millis();
    digitalWrite(this->pinNumber, this->state);
  }

  void LED::flash(unsigned int interval) {
    this->mode = LedMode::LED_FLASH;
    this->interval = interval;
    this->state = HIGH;
    this->time = millis();
    digitalWrite(this->pinNumber, this->state);
  }

  void LED::touch() {
    this->touch(millis());
  }

  void LED::touch(unsigned long now) {
    if ((this->mode != LedMode::LED_BLINK) && (this->mode != LedMode::LED_FLASH)) {
      return;
    }
    if (now - this->time >= this->interval) {
      if (this->mode == LedMode::LED_FLASH) {
        this->state = LOW;
        this->mode = LedMode::LED_OFF;
        digitalWrite(this->pinNumber, this->state);
        return;
      }
      this->state = (this->state == HIGH) ? LOW : HIGH;
      this->time = now;
      digitalWrite(this->pinNumber, this->state);
    }
  }

}