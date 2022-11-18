#include <Arduino.h>
#include <FastLED.h>
#include "microLED.h"
#include "LED.h"
#include "LedManager.v1.h"

FASTLED_USING_NAMESPACE

#ifdef BOARD_ATTINY85
#define DATA_PIN    3
#define NUM_LEDS    50
#define BRIGHTNESS  255
#else
#define DATA_PIN    13
#define NUM_LEDS    30
#define BRIGHTNESS  100
#endif
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define FRAMES_PER_SECOND  30
#define GREENLED_PIN 4

#define BUTTON_PIN 0

microLED<NUM_LEDS, DATA_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_HIGH> strip;
Nezumikun::LedManager ledManager((ImicroLED *) &strip, NUM_LEDS, FRAMES_PER_SECOND);
Nezumikun::LED ledGreen(GREENLED_PIN);

void callbackAllModes() {
  ledGreen.blink(500);
}

void setup() {
#ifdef BOARD_ARDUINO_NANO
  Serial.begin(115200);
  randomSeed(analogRead(0));
  Serial.println("Begin");
#endif
  pinMode(BUTTON_PIN, INPUT);
  delay(500); // delay for recovery
  ledGreen.on();
  ledManager.setAllModesCallback(callbackAllModes);
  ledManager.begin();
}

void checkButtonPush(unsigned long now) {
  static bool hold = false;
  static bool buttonState = false;
  static uint32_t buttonTimer = 0;
  bool temp = digitalRead(BUTTON_PIN);
  uint32_t delta = now - buttonTimer;
  if (temp && !buttonState && delta > 100) {
    buttonState = true;
    buttonTimer = now;
    hold = false;
  }
  else if (temp && buttonState && delta > 1000) {
    ledManager.setDemoMode(true);
    ledGreen.on();
    hold = true;
  }
  else if (!temp && buttonState && delta > 100) {
    buttonState = false;
    buttonTimer = now;
    if (!hold) {
      ledManager.setDemoMode(false);
      ledGreen.off();
      ledManager.nextPattern(false);
    }
    hold = false;
  }
}

void loop()
{
  unsigned long now = millis();
  #ifndef BOARD_ARDUINO_NANO
  checkButtonPush(now);
  #endif
  ledGreen.touch(now);
  ledManager.loop(now);
}
