#pragma once

#include "Hardware.h"
#include "LayoutInterface.h"
#include <SC_Button.h>

class LayoutMidi : public LayoutInterface {

  friend class InputManager;

public:
  const char *getName() { return "MIDI"; }

protected:
  enum Buttons {
    CC1_9,     // 0
    CC2_10,    // 1
    CC3_11,    // 2
    CC4_12,    // 3
    ModeTuner, // 4
    CC5_13,    // 5
    CC6_14,    // 6
    CC7_15,    // 7
    CC8_16,    // 8
    Tap        // 9
  };

  using LayoutInterface::LayoutInterface;

  void reset() { _leds->dimAll(); }

  bool readButton(const byte index, Button &button) {

    int8_t cc = -1;
    byte data = 1;

    byte idx = index < ModeTuner ? index + 1 : index;
    if (HOLD_LONG) {
      cc = idx + 8;
    } else if (RELEASE) {
      cc = idx;
    }

    if (cc > -1) {
      _axe->sendControlChange(cc, data, _axe->getMidiChannel());
      return true;
    }

    return false;
  }
};
