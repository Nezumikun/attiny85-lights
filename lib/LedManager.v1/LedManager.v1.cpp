#include "LedManager.v1.h"

namespace Nezumikun {
  LedManager::LedManager(CRGB* leds, uint8_t ledsNumber, uint8_t framesPerSecond, bool demoMode) {
    this->leds = leds;
    this->ledsNumber = ledsNumber;
    this->timeInterval = 1000 / (uint16_t)framesPerSecond;
    this->demoMode = demoMode;
    this->x = random16();
    this->y = random16();
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
      this->currentEffectNumber = 0;
      this->subMode = 0;
      this->isShowAllModesInDemo = false;
    }
  }

  void LedManager::effectRainbow() {
    // FastLED's built-in rainbow generator
    fill_rainbow(this->leds, this->ledsNumber, this->hue + inoise8(this->x, this->y), 255 / this->ledsNumber);
    if(((this->subMode & 1) == 1) && (random8() < 80)) {
      this->leds[random16(this->ledsNumber)] += CRGB::White;
    }
    this->x++;
    this->y++;
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

  #ifdef BOARD_ATTINY85
  #define NEZUMIKUN_LED_MANAGER_PALLETES_NUMBER 2
  #else
  #define NEZUMIKUN_LED_MANAGER_PALLETES_NUMBER 8
  DEFINE_GRADIENT_PALETTE ( aurora_gp ) {
    0,  17, 177,  13,    //Greenish
    64, 121, 242,   5,    //Greenish
    128,  25, 173, 121,    //Turquoise
    192, 250,  77, 127,    //Pink
    255, 171, 101, 221     //Purple
  };
  #endif
  DEFINE_GRADIENT_PALETTE( xmas_gp ) {
    0,   0, 12,  0,
    40,   0, 55,  0,
    66,   1, 117,  2,
    77,   1, 84,  1,
    81,   0, 55,  0,
    119,   0, 12,  0,
    153,  42,  0,  0,
    181, 121,  0,  0,
    204, 255, 12,  8,
    224, 121,  0,  0,
    244,  42,  0,  0,
    255,  42,  0,  0
  };
  void LedManager::effectPerlinNoise() {
    for(uint8_t i = 0; i < this->ledsNumber; i++) {
      switch(this->subMode % NEZUMIKUN_LED_MANAGER_PALLETES_NUMBER) {
        case 0:
          leds[i] = ColorFromPalette((CRGBPalette16) xmas_gp, inoise8(i * 40, this->y));
          break;
        case 1:
          leds[i] = ColorFromPalette(HeatColors_p, inoise8(i * 40, this->y));
          break;
#ifndef BOARD_ATTINY85
        case 2:
          leds[i] = ColorFromPalette(PartyColors_p, inoise8(i * 40, this->y));
          break;
        case 3:
          leds[i] = ColorFromPalette(RainbowColors_p, inoise8(i * 40, this->y));
          break;
        case 4:
          leds[i] = ColorFromPalette(LavaColors_p, inoise8(i * 40, this->y));
          break;
        case 5:
          leds[i] = ColorFromPalette(CloudColors_p, inoise8(i * 40, this->y));
          break;
        case 6:
          leds[i] = ColorFromPalette((CRGBPalette16) aurora_gp, inoise8(i * 40, this->y));
          break;
        case 7:
          leds[i] = CHSV(this->hue + inoise8(this->x + i * 50, this->y), 255, inoise8(this->y + i * 50, this->x));
          break;
#endif
      }
    }
    this->x++;
    this->y += 10;
  }


  void LedManager::nextEffect() {
    NANO_PRINT("Next Effect: ");
    NANO_PRINT(this->currentEffectNumber);
    NANO_PRINT(" -> ");
    if (this->demoMode) {
      NANO_PRINT("[DEMO] ");
      if (!this->isShowAllModesInDemo) {
        NANO_PRINT("[SHOW ALL] ");
        this->currentEffectNumber = (this->currentEffectNumber + 1) % NEZUMIKUN_LED_MANAGER_EFFECTS_NUMBER;
        NANO_PRINT(this->currentEffectNumber);
        if (this->currentEffectNumber == 0) {
          this->isShowAllModesInDemo = true;
          if (this->callbackAllModes != NULL) {
            this->callbackAllModes();
          }
          NANO_PRINT(" [SHOW ALL OFF]");
        }
      } else {
        if ((random8() & 1) == 1) {
          this->subMode = random8() % NEZUMIKUN_LED_MANAGER_PALLETES_NUMBER;
          this->currentEffectNumber = random8(NEZUMIKUN_LED_MANAGER_EFFECTS_NUMBER);
        }
        NANO_PRINT(this->currentEffectNumber);
        if (this->currentEffectNumber == 5) {
          NANO_PRINT(" (");          
          NANO_PRINT(this->subMode);
          NANO_PRINT(")");          
        }
      }
    } else {
      this->subMode = random8() % NEZUMIKUN_LED_MANAGER_PALLETES_NUMBER;
      this->currentEffectNumber = (this->currentEffectNumber + 1) % NEZUMIKUN_LED_MANAGER_EFFECTS_NUMBER;
      NANO_PRINT(this->currentEffectNumber);
      if (this->currentEffectNumber == 5) {
        NANO_PRINT(" (");          
        NANO_PRINT(this->subMode);
        NANO_PRINT(")");          
      }
    }
    NANO_PRINTLN("");
  }

  void LedManager::setAllModesCallback(ptrAllModesCallBack callback) {
    this->callbackAllModes = callback;
  }

  void LedManager::nextPalette() {
    uint8_t subMode = this->subMode;
    this->subMode = random8() % NEZUMIKUN_LED_MANAGER_PALLETES_NUMBER;
    if (this->subMode != subMode) {
      NANO_PRINT("Set new palette  = ");          
      NANO_PRINT(this->subMode);
      NANO_PRINTLN("");          
    }
  }

  void LedManager::loop(unsigned long now) {
    if (now - this->prevTime >= this->timeInterval) {
      // Call the current pattern function once, updating the 'leds' array
      switch(this->currentEffectNumber) {
        case 0: this->effectRainbow(); break;
        case 1: this->effectConfetti(); break;
        case 2: this->effectSinelon(); break;
        case 3: this->effectJuggle(); break;
        case 4: this->effectBpm(); break;
        case 5: this->effectPerlinNoise(); break;
      }
      // this->subMode = 0;
      // this->effectPerlinNoise();

      // send the 'leds' array out to the actual LED strip
      FastLED.show();
      this->prevTime = now;
    }
    EVERY_N_MILLISECONDS(20) { this->hue++; } // slowly cycle the "base color" through the rainbow
    EVERY_N_SECONDS(20) { 
      if (this->demoMode) {
        this->nextEffect();
      } // change patterns periodically
      else {
        if (this->currentEffectNumber == 5) {
          this->nextPalette();
        }
      }
    }
  }

}