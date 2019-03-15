#include <SC_Multiplexer.h>
#include <SC_Button.h>
#include "InputManager.h"
#include "LayoutScenes.h"
#include "LayoutLooper.h"
#include "LayoutMidi.h"
#include "LayoutKitchenSink.h"
#include "LayoutPedals.h"

void InputManager::init(AxeSystem& axe, Leds& leds, Screen& screen) {
  
  _axe = &axe;
  _leds = &leds;

	_leds->dim(Leds::MODE_TUNER_LED);
	
  _layouts[Pedals] = new LayoutPedals(_axe, this, _leds, &screen);
  _layouts[Scenes] = new LayoutScenes(_axe, this, _leds, &screen);
	// _layouts[Midi]   = new LayoutMidi(_axe, this, _leds);
  // _layouts[Looper] = new LayoutLooper(_axe, this, _leds);
  // _layouts[KitchenSink] = new LayoutKitchenSink(_axe, this, _leds);

  _layoutType = Pedals;

  for (byte i=0; i<NUM_BUTTONS; i++) {
    byte pin = BUTTON1_PIN - i;
    pinMode(pin, INPUT_PULLUP);
    _buttons[i].setPin(pin);
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
    if (getLayout()->read(i, _buttons[i])) {
      changed = true;
    }
  }

  return changed;

}

void InputManager::nextLayout() {
  setLayoutType( static_cast<LayoutType>((_layoutType + 1) % __NUM_LAYOUT_TYPES) );
}

void InputManager::setLayoutType(LayoutType layout) { 
  _layoutType = layout; 
	getLayout()->reset();
  callLayoutChangeCallback(); 
}

void InputManager::callLayoutChangeCallback() {
  if (NULL != _layoutChangeCallback) {
    (_layoutChangeCallback)(getLayout());
  }
}
