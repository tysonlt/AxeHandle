#pragma once

#include "Hardware.h"
#include "LayoutInterface.h"
#include <SC_Button.h>

class LayoutPedals : public LayoutInterface {

  friend class InputManager;

public:
  const char *getName() { return "Pedals"; }

  void reset() { updateEffectLeds(); }

protected:
  enum Buttons {
    PresetSceneUp,   // 0
    Drive2,          // 1
    Delay2,          // 2
    Mod1,            // 3
    ModeTuner,       // 4
    PresetSceneDown, // 5
    Drive1,          // 6
    Delay1,          // 7
    Wah1,            // 8
    Tap              // 9
  };

  using LayoutInterface::LayoutInterface;

  bool readButton(const byte index, Button &button) {

    switch (index) {

    case PresetSceneUp:
      if (HOLD) {
        _axe->sendPresetDecrement();
        return true;
      } else if (RELEASE) {
        _axe->sendSceneDecrement();
        return true;
      }
      break;

    case PresetSceneDown:
      if (HOLD) {
        _axe->sendPresetIncrement();
        return true;
      } else if (RELEASE) {
        _axe->sendSceneIncrement();
        return true;
      }
      break;

    default:
      return processEffect(index, button);
    };

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

  void updateEffectLeds() {
    setEffectLedState(ID_FUZZ1);
    setEffectLedState(ID_FUZZ2);
    setEffectLedState(ID_DELAY1);
    setEffectLedState(ID_DELAY2);
    setEffectLedState(ID_WAH1);
    setEffectLedState(ID_CHORUS1);
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
    case Drive2:
      return ID_FUZZ2;
    case Delay1:
      return ID_DELAY1;
    case Delay2:
      return ID_DELAY2;
    case Wah1:
      return ID_WAH1;
    case Mod1:
      return ID_CHORUS1;
    default:
      return 0; // to stop compiler warnings
    }
  }

  byte indexForEffectId(const EffectId effectId) {
    switch (effectId) {
    case ID_FUZZ1:
      return Drive1;
    case ID_FUZZ2:
      return Drive2;
    case ID_DELAY1:
      return Delay1;
    case ID_DELAY2:
      return Delay2;
    case ID_WAH1:
      return Wah1;
    case ID_CHORUS1:
      return Mod1;
    default:
      return 0; // to stop compiler warnings
    }
  }

  int getEffectOrder(const EffectId effectId) {
    switch (effectId) {
    case ID_FUZZ1:
      return 1;
    case ID_FUZZ2:
      return 2;
    case ID_DELAY1:
      return 3;
    case ID_DELAY2:
      return 4;
    case ID_WAH1:
      return 5;
    case ID_CHORUS1:
      return 6;
    }
    return 0;
  }
};
