#pragma once

#include <AxeFxControl.h>
#include <SC_Button.h>
#include "InputManager.h"
#include "Leds.h"

#define PRESS button.wasPressed()
#define HOLD buttonHeld(button, index)
#define RELEASE buttonReleased(button, index)

class LayoutInterface {

  friend class InputManager;

  public:

    virtual void update() =0;

  protected:

    static const byte BUTTON_TAP = 9;
    static const byte BUTTON_MODE_TUNER = 4;

    LayoutInterface(AxeSystem *axe, InputManager *input, Leds *leds) {
      _axe = axe;
      _input = input;
      _leds = leds;
    }

    virtual bool readButton(byte index, Button& button) =0;

    bool processStandardButtons(byte index, Button& button) {

      switch (index) {

        case BUTTON_TAP:
          if (PRESS) {
            _axe->sendTap();
            return true;
          }
          break;

        case BUTTON_MODE_TUNER:
          if (HOLD) {
            _input->nextLayout();
            return true;
          } else if (RELEASE) {
            _axe->toggleTuner();
            return true;
          }
          break;

      };

      return false;

    }

    bool buttonHeld(Button& button, byte index, millis_t millis = BUTTON_HOLD_MILLIS) {
      if (button.pressedFor(millis) && !_holdState[index]) {
        _holdState[index] = true;
        return true;
      }
      return false;
    }

    bool buttonReleased(Button& button, byte index) {
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
    bool _holdState[NUM_BUTTONS] = {false};

};