#include <SC_Multiplexer.h>
#include <SC_Button.h>
#include "InputManager.h"
#include "ControlSchemeStandard.h"
#include "ControlSchemeLooper.h"
#include "ControlSchemeKitchenSink.h"

void InputManager::init(AxeSystem& axe) {
  
  _axe = &axe;

  _controlSchemes[Standard] = new ControlSchemeStandard;
  _controlSchemes[Looper] = new ControlSchemeLooper;
  _controlSchemes[KitchenSink] = new ControlSchemeKitchenSink;

  setControlScheme(Standard);

  _mux.setPins(MUX0_PIN0, MUX0_PIN1, MUX0_PIN2, MUX0_PIN3);
  pinMode(MUX0_SIG_PIN, INPUT_PULLUP);

  for (byte i=0; i<NUM_BUTTONS; i++) {
    _buttons[i].setPin(MUX0_SIG_PIN);
    _buttons[i].setMultiplexer(&_mux, i);
    _buttons[i].setDebounceTime(BUTTON_DEBOUNCE[i]);
    _buttons[i].setInverted(BUTTON_INVERTED[i]);
    _buttons[i].begin();
  }

}

bool InputManager::update() {
  
  bool changed = false;

  for (byte i=0; i<NUM_BUTTONS; i++) {
    if (getControlScheme()->readButton(i, _buttons[i])) {
      changed = true;
    }
  }

  return changed;

}

void InputManager::nextControlScheme() {
  _schemeType = static_cast<ControlSchemeType>((_schemeType + 1) % __NUM_CONTROL_SCHEME_TYPES);
}