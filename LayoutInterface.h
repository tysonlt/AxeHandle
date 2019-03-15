#pragma once

#include "Hardware.h"
#include "InputManager.h"
#include "Leds.h"
#include "Screen.h"
#include <AxeFxControl.h>
#include <SC_Button.h>

#define PRESS button.wasPressed()
#define HOLD buttonHeld(button, index, BUTTON_HOLD_MILLIS)
#define HOLD_SHORT buttonHeld(button, index, BUTTON_SHORT_HOLD_MILLIS)
#define HOLD_LONG buttonHeld(button, index, BUTTON_LONG_HOLD_MILLIS)
#define RELEASE buttonReleased(button, index)

class LayoutInterface {

public:
  virtual const char *getName() = 0;
  virtual void reset() = 0;

  bool read(const byte index, Button &button) {
    if (processStandardButtons(index, button)) {
      return true;
    }
    return readButton(index, button);
  }

protected:
  static const byte BUTTON_TAP = 9;
  static const byte BUTTON_MODE_TUNER = 4;

  LayoutInterface(AxeSystem *axe, InputManager *input, Leds *leds, Screen *screen) {
    _axe = axe;
    _input = input;
    _leds = leds;
    _screen = screen;
  }

  virtual bool readButton(const byte index, Button &button) = 0;

  bool buttonHeld(Button &button, const byte index, const millis_t millis = BUTTON_HOLD_MILLIS) {
    if (button.pressedFor(millis) && !_holdState[index]) {
      _holdState[index] = true;
      return true;
    }
    return false;
  }

  bool buttonReleased(Button &button, const byte index) {
    bool released = button.wasReleased();
    if (released && _holdState[index]) {
      _holdState[index] = false;
      return false;
    }
    return released;
  }

  AxeSystem *_axe = nullptr;
  InputManager *_input = nullptr;
  Leds *_leds = nullptr;
  Screen *_screen = nullptr;
  bool _holdState[NUM_BUTTONS] = {false};

private:
  bool processStandardButtons(const byte index, Button &button) {

    switch (index) {

    case BUTTON_TAP:
      if (PRESS) {
        _axe->sendTap();
      }
      return true;

    case BUTTON_MODE_TUNER:

      if (_axe->isTunerEngaged()) {
        if (PRESS) {
          _axe->toggleTuner();
        }
      } else {
        if (HOLD) {
          _axe->enableTuner();
        } else if (RELEASE) {
          _input->nextLayout();
        }
      }

      return true;
    };

    return false;
  }
};