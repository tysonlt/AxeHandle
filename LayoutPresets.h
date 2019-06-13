#pragma once

#include "LayoutInterface.h"
#include <SC_Button.h>

class LayoutPresets : public LayoutInterface {

  friend class InputManager;

public:
  const char *getName() { return "Presets"; }

  void reset() { turnOnPresetLed(_axe->getCurrentPreset().getPresetNumber()); }

protected:
  enum Buttons {
    Preset4,    // 0
    Preset5,    // 1
    Preset6,    // 2
    Preset7,    // 3
    ModeTuner,  // 4
    Preset0,    // 5
    Preset1,    // 6
    Preset2,    // 7
    Preset3,    // 8
    Tap         // 9
  };

  using LayoutInterface::LayoutInterface;

  bool readButton(const byte index, Button &button) {

    if (button.wasPressed()) {
      PresetNumber preset = indexToPreset(index);
      _axe->sendPresetChange(preset);
      turnOnPresetLed(preset);
      return true;
    }

    return false;
  }

  void turnOnPresetLed(PresetNumber preset) {
    _leds->dimAll();
    if (preset < 8) {
      _leds->on(presetToIndex(preset));
    }
  }

  PresetNumber indexToPreset(byte index) {
    return index < ModeTuner ? index + 4 : index - 5;
  }

  byte presetToIndex(PresetNumber preset) {
    return preset < ModeTuner ? preset - 4 : preset + 5;
  }

};
