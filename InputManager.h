#pragma once

#include <Arduino.h>
#include <AxeFxControl.h>
#include <SC_Multiplexer.h>
#include <SC_Button.h> 
#include "Hardware.h"
#include "ControlScheme.h"

enum ControlSchemeType {
  Standard,
  Looper,
  KitchenSink,
  __NUM_CONTROL_SCHEME_TYPES
};

class InputManager {

  public:

    void init(AxeSystem& axe);

    bool update();

    void setControlScheme(ControlSchemeType scheme) { _schemeType = scheme; }

    void nextControlScheme();

  private:

    ControlScheme* getControlScheme() {
      return _controlSchemes[_schemeType];
    }

    AxeSystem *_axe = nullptr;
    Button _buttons[NUM_BUTTONS];
    Multiplexer _mux;
    ControlSchemeType _schemeType;
    ControlScheme *_controlSchemes[__NUM_CONTROL_SCHEME_TYPES];

};