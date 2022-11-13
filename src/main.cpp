#include <Arduino.h>
#include <FastLED.h>
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
CRGB leds[NUM_LEDS];

#define FRAMES_PER_SECOND  50
#define GREENLED_PIN 4

#define BUTTON_PIN 0

Nezumikun::LedManager ledManager(&leds[0], NUM_LEDS, FRAMES_PER_SECOND);

void setup() {
#ifdef BOARD_ARDUINO_NANO
  Serial.begin(115200);
#endif  
  pinMode(BUTTON_PIN, INPUT);
  pinMode(GREENLED_PIN, OUTPUT);
  delay(500); // delay for recovery
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
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
    hold = true;
  }
  else if (!temp && buttonState && delta > 100) {
    buttonState = false;
    buttonTimer = now;
    if (!hold) {
      ledManager.setDemoMode(false);
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
  digitalWrite(GREENLED_PIN, ledManager.isDemoMode() ? HIGH : LOW);
  #endif
  ledManager.loop(now);
}
