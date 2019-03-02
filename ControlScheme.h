#pragma once

#include <AxeFxControl.h>
#include <SC_Button.h>

#define PRESS button.wasPressed()
#define HOLD buttonHeld(button, index)
#define RELEASE buttonReleased(button, index)

class InputManager;
class ControlScheme {

  public:

    ControlScheme(AxeSystem *axe, InputManager *input) {
      _axe = axe;
      _input = input;
    }

    virtual bool readButton(byte index, Button& button) =0;

  protected:

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
    bool _holdState[NUM_BUTTONS] = {false};

};