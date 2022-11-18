#include "LedManager.v1.h"

namespace Nezumikun {
  LedManager::LedManager(ImicroLED* strip, uint8_t ledsNumber, uint8_t framesPerSecond, bool demoMode) {
    this->strip = strip;
    this->ledsNumber = ledsNumber;
    this->timeInterval = 1000 / (uint16_t)framesPerSecond;
    this->demoMode = demoMode;
    this->x = random16();
    this->y = random16();
    this->z = random16();
  }

  void LedManager::begin() {
    this->prevTime -= this->timeInterval;
  }

  bool LedManager::isDemoMode() {
    return this->demoMode;
  }

  void LedManager::setDemoMode(bool demoMode) {
    this->demoMode = demoMode;
    if (demoMode) {
      this->currentPatternNumber = 0;
      this->isShowAllModesInDemo = false;
    }
  }

  void LedManager::effectRainbow() {
    for (int i = 0; i < this->ledsNumber; i++) {
      strip->send(mWheel8((this->currentIndex + i) * 255 / this->ledsNumber)); // выводим радугу
    }
    this->currentIndex += this->currentIndex + this->forward ? 1 : -1;
    this->currentIndex %= this->ledsNumber;
  }

  void LedManager::effectRainbowWithGlitter() {
    int16_t glitter = -1;
    if( random8() < 80) {
      glitter = random16(this->ledsNumber);
    }
    for (int i = 0; i < this->ledsNumber; i++) {
      strip->send((glitter >= 0 && glitter == i) ? mWhite : mWheel8((this->currentIndex + i) * 255 / this->ledsNumber)); // выводим радугу
    }
    this->currentIndex += this->currentIndex + this->forward ? 1 : -1;
    this->currentIndex %= this->ledsNumber;
  }

  void LedManager::effectConfetti() {
    // random colored speckles that blink in and fade smoothly
    //fadeToBlackBy(this->leds, this->ledsNumber, 10);
    int pos = random16(this->ledsNumber);
    //this->leds[pos] += CHSV(hue + random8(64), 200, 255);
  }

  void LedManager::effectSinelon() {
    // a colored dot sweeping back and forth, with fading trails
    //fadeToBlackBy(this->leds, this->ledsNumber, 20);
    int pos = beatsin16(13, 0, this->ledsNumber - 1);
    //this->leds[pos] += CHSV(this->hue, 255, 192);
  }

  void LedManager::effectBpm() {
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
    for( int i = 0; i < this->ledsNumber; i++) { //9948
      //leds[i] = ColorFromPalette(palette, this->hue + (i*2), beat - this->hue +(i*10));
    }
  }

  void LedManager::effectJuggle() {
    // eight colored dots, weaving in and out of sync with each other
    //fadeToBlackBy(this->leds, this->ledsNumber, 20);
    uint8_t dothue = 0;
    for( int i = 0; i < 8; i++) {
      //leds[beatsin16(i + 7, 0, this->ledsNumber-1 )] |= CHSV(dothue, 200, 255);
      dothue += 32;
    }
  }

  void LedManager::effectPerlinNoise() {
    // eight colored dots, weaving in and out of sync with each other
    for(uint8_t i = 0; i < this->ledsNumber; i++) {
      //leds[i] = CHSV(this->hue + inoise8(this->x + i * 50, this->y, this->z), 255, inoise8(this->y + i * 50, this->x, this->z));
    }
    this->x++;
    this->y--;
    this->z++;
  }


  void LedManager::nextPattern(bool autoChange) {
    if (this->demoMode) {
      if (!this->isShowAllModesInDemo) {
        this->currentPatternNumber = (this->currentPatternNumber + 1) % NEZUMIKUN_LED_MANAGER_EFFECTS_NUMBER;
        if (this->currentPatternNumber == 0) {
          this->isShowAllModesInDemo = true;
          if (this->callbackAllModes != NULL) {
            this->callbackAllModes();
          }
        }
      } else {
        if ((random8() & 1) == 1) {
          this->currentPatternNumber = random8(NEZUMIKUN_LED_MANAGER_EFFECTS_NUMBER);
          this->forward = (random8() & 1) == 1;
        }
      }
    } else {
      this->currentPatternNumber = (this->currentPatternNumber + 1) % NEZUMIKUN_LED_MANAGER_EFFECTS_NUMBER;
    }
  }

  void LedManager::setAllModesCallback(ptrAllModesCallBack callback) {
    this->callbackAllModes = callback;
  }

  void LedManager::loop(unsigned long now) {
    if (now - this->prevTime >= this->timeInterval) {
      // Call the current pattern function once, updating the 'leds' array
      this->strip->begin();
      switch(this->currentPatternNumber) {
        case 0: this->effectRainbow(); break;
        case 1: this->effectRainbowWithGlitter(); break;
        case 2: this->effectConfetti(); break;
        case 3: this->effectSinelon(); break;
        case 4: this->effectJuggle(); break;
        case 5: this->effectBpm(); break;
        case 6: this->effectPerlinNoise(); break;
      }
      this->strip->end();

      // send the 'leds' array out to the actual LED strip
      FastLED.show();
      this->prevTime = now;
    }
    EVERY_N_MILLISECONDS(20) { this->hue++; } // slowly cycle the "base color" through the rainbow
    if (this->demoMode) {
      EVERY_N_SECONDS(20) { this->nextPattern(true); } // change patterns periodically
    }
  }

}