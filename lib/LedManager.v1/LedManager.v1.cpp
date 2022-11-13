#include "LedManager.v1.h"

namespace Nezumikun {
  LedManager::LedManager(CRGB* leds, uint8_t ledsNumber, uint8_t framesPerSecond, bool demoMode) {
    this->leds = leds;
    this->ledsNumber = ledsNumber;
    this->framesPerSecond = framesPerSecond;
    this->demoMode = demoMode;
  }

  void LedManager::begin() {
    this->prevTime -= 1000 / framesPerSecond;
  }

  void LedManager::loop(unsigned long now) {
    if (now - this->prevTime >= 1000/this->framesPerSecond) {
      // Call the current pattern function once, updating the 'leds' array
      switch(this->currentPatternNumber) {
        case 0: this->effectRainbow(); break;
        case 1: this->effectRainbowWithGlitter(); break;
        case 2: this->effectConfetti(); break;
        case 3: this->effectSinelon(); break;
        case 4: this->effectJuggle(); break;
        case 5: this->effectBpm(); break;
      }

      // send the 'leds' array out to the actual LED strip
      FastLED.show();
      this->prevTime = now;
    }
    EVERY_N_MILLISECONDS(20) { this->hue++; } // slowly cycle the "base color" through the rainbow
    if (this->demoMode) {
      EVERY_N_SECONDS(20) { this->nextPattern(true); } // change patterns periodically
    }
  }

  bool LedManager::isDemoMode() {
    return this->demoMode;
  }

  void LedManager::setDemoMode(bool demoMode) {
    this->demoMode = demoMode;
    if (demoMode) {
      this->currentPatternNumber = 0;
    }
  }

  void LedManager::effectRainbow() {
    // FastLED's built-in rainbow generator
    fill_rainbow(this->leds, this->ledsNumber, this->hue, 7);
  }

  void LedManager::toolsAddGlitter(fract8 chanceOfGlitter) {
    if( random8() < chanceOfGlitter) {
      this->leds[random16(this->ledsNumber)] += CRGB::White;
    }
  }

  void LedManager::effectRainbowWithGlitter() {
    // built-in FastLED rainbow, plus some random sparkly glitter
    this->effectRainbow();
    this->toolsAddGlitter(80);
  }

  void LedManager::effectConfetti() {
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy(this->leds, this->ledsNumber, 10);
    int pos = random16(this->ledsNumber);
    this->leds[pos] += CHSV(hue + random8(64), 200, 255);
  }

  void LedManager::effectSinelon() {
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy(this->leds, this->ledsNumber, 20);
    int pos = beatsin16(13, 0, this->ledsNumber - 1);
    this->leds[pos] += CHSV(this->hue, 255, 192);
  }

  void LedManager::effectBpm() {
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
    for( int i = 0; i < this->ledsNumber; i++) { //9948
      leds[i] = ColorFromPalette(palette, this->hue + (i*2), beat - this->hue +(i*10));
    }
  }

  void LedManager::effectJuggle() {
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy(this->leds, this->ledsNumber, 20);
    uint8_t dothue = 0;
    for( int i = 0; i < 8; i++) {
      leds[beatsin16(i + 7, 0, this->ledsNumber-1 )] |= CHSV(dothue, 200, 255);
      dothue += 32;
    }
  }

  void LedManager::nextPattern(bool autoChange) {
    // add one to the current pattern number, and wrap around at the end
    this->currentPatternNumber = (this->currentPatternNumber + 1) % NEZUMIKUN_LED_MANAGER_EFFECTS_NUMBER;
  }

}