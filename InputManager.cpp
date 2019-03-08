#include <SC_Multiplexer.h>
#include <SC_Button.h>
#include "InputManager.h"
#include "LayoutScenes.h"
#include "LayoutLooper.h"
#include "LayoutKitchenSink.h"
#include "LayoutPedals.h"

void InputManager::init(AxeSystem& axe, Leds& leds) {
  
  _axe = &axe;
  _leds = &leds;

  _layouts[Pedals] = new LayoutPedals(_axe, this, _leds);
  _layouts[Scenes] = new LayoutScenes(_axe, this, _leds);
  _layouts[Looper] = new LayoutLooper(_axe, this, _leds);
  _layouts[KitchenSink] = new LayoutKitchenSink(_axe, this, _leds);

  setLayoutType(Pedals);

  _mux.setPins(MUX0_PIN0, MUX0_PIN1, MUX0_PIN2, MUX0_PIN3);
  pinMode(MUX0_SIG_PIN, INPUT_PULLUP);

  for (byte i=0; i<NUM_BUTTONS; i++) {
    _buttons[i].setPin(MUX0_SIG_PIN);
    _buttons[i].setMultiplexer(&_mux, i);
    _buttons[i].setDebounceTime(BUTTON_DEBOUNCE[i]);
    _buttons[i].setInverted(BUTTON_INVERTED[i]);
    _buttons[i].setPullupEnable(BUTTON_PULLUP[i]);
    _buttons[i].begin();
  }

}

bool InputManager::update() {

  bool changed = false;
  for (byte i=0; i<NUM_BUTTONS; i++) {
    _buttons[i].read();
    if (getLayout()->readButton(i, _buttons[i])) {
      changed = true;
    }
  }

  return changed;

}

void InputManager::updateLeds() { 
  getLayout()->update(); 
}

void InputManager::nextLayout() {
  setLayoutType( static_cast<LayoutType>((_layoutType + 1) % __NUM_LAYOUT_TYPES) );
}

void InputManager::setLayoutType(LayoutType layout) { 
  _layoutType = layout; 
  callLayoutChangeCallback(); 
}

void InputManager::callLayoutChangeCallback() {
  if (NULL != _layoutChangeCallback) {
    (_layoutChangeCallback)(_layoutType);
  }
}
