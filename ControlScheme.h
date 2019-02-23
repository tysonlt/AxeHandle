#pragma once

#include <AxeFxControl.h>
#include <SC_Button.h>

class InputManager;
class ControlScheme {

  public:

    ControlScheme(AxeSystem *axe, InputManager *input) {
      _axe = axe;
      _input = input;
    }

    virtual bool readButton(byte index, Button& button) =0;

  protected:

    AxeSystem *_axe = nullptr;
    InputManager *_input = nullptr;

};