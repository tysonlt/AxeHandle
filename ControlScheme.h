#pragma once

#include <AxeFxControl.h>
#include <SC_Button.h>

class InputManager;
class ControlScheme {

  public:

    virtual bool readButton(byte index, Button& button) =0;

  protected:

    void setAxeSystem(AxeSystem& axe) { _axe = &axe; }
    void setInputManager(InputManager& input) { _input = &input; }

    AxeSystem *_axe = nullptr;
    InputManager *_input = nullptr;

};