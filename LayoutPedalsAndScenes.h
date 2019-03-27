#pragma once

#include "Hardware.h"
#include "LayoutInterface.h"
#include <SC_Button.h>

class LayoutPedalsAndScenes : public LayoutInterface {

  friend class InputManager;

public:
  const char *getName() { return "Scene/Pedal"; }

  void reset() { 
    updateEffectLeds(); 
    turnOnSceneLed( _axe->getCurrentPreset().getSceneNumber() );
  }

protected:
  enum Buttons {
    Scene1,          // 0
    Scene2,          // 1
    Scene3,          // 2
    Scene4,          // 3
    ModeTuner,       // 4
    Drive1,          // 5
    Delay1,          // 6
    Chorus1,         // 7 
    Tremelo1,        // 8
    Tap              // 9
  };

  using LayoutInterface::LayoutInterface;

  bool readButton(const byte index, Button &button) {

      if (index >= Scene1 && index <= Scene4) {
        if (HOLD) {
          _axe->sendSceneChange(index + 5);
          turnOnSceneLed(index); //reuse led
          return true;
        } else if (RELEASE) {
          _axe->sendSceneChange(index + 1);
          turnOnSceneLed(index);
          return true;
        }
      } else {
        return processEffect(index, button);
      }

    return false;
  }

private:
  bool processEffect(const byte index, Button &button) {
    EffectId effectId = effectIdFor(index);
    if (PRESS) {
      if (AxeEffect *effect = _axe->getCurrentPreset().getEffectById(effectId)) {
        effect->toggle();
        updateEffectLeds();
        return true;
      }
    }
    return false;
  }

  void turnOnSceneLed(SceneNumber scene) {
    _leds->dim(0);
    _leds->dim(1);
    _leds->dim(2);
    _leds->dim(3);
    if (scene <= 4) {
      _leds->on(scene - 1);
    }
  }

  void updateEffectLeds() {
    setEffectLedState(ID_FUZZ1);
    setEffectLedState(ID_DELAY1);
    setEffectLedState(ID_CHORUS1);
    setEffectLedState(ID_TREMOLO1);
  }

  void setEffectLedState(const EffectId effectId) {
    byte index = indexForEffectId(effectId);
    if (AxeEffect *effect = _axe->getCurrentPreset().getEffectById(effectId)) {
      effect->isBypassed() ? _leds->dim(index) : _leds->on(index);
    } else {
      _leds->off(index);
    }
  }

  EffectId effectIdFor(const byte index) {
    switch (index) {
    case Drive1:
      return ID_FUZZ1;
    case Delay1:
      return ID_DELAY1;
    case Chorus1:
      return ID_CHORUS1;
    case Tremelo1:
      return ID_TREMOLO1;
    default:
      return 0; // to stop compiler warnings
    }
  }

  byte indexForEffectId(const EffectId effectId) {
    switch (effectId) {
    case ID_FUZZ1:
      return Drive1;
    case ID_DELAY1:
      return Delay1;
    case ID_CHORUS1:
      return Chorus1;
    case ID_TREMOLO1:
      return Tremelo1;
    default:
      return 0; // to stop compiler warnings
    }
  }

};
