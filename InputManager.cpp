#include <SC_Multiplexer.h>
#include <SC_Button.h>
#include "InputManager.h"
#include "LayoutScenes.h"
#include "LayoutPedalsAndScenes.h"
#include "LayoutPedals.h"
#include "LayoutPresets.h"
#include "LayoutMidi.h"
#include "LayoutLooper.h"
#include "LayoutUser.h"
#include "LayoutSetup.h"
#include "LayoutScenesAndMidi.h"

void InputManager::checkSetupMode() {

  #ifdef TESTING_SETUP
  _setupMode = true;
  while (true) update();
  #endif

  for (byte i=0; i<NUM_BUTTONS; i++) {
    if (_buttons[i].isPressed()) {
      _setupMode = true;
      while (true) update();
    }
  }
  
}

void InputManager::init(AxeSystem& axe, Leds& leds, Screen& screen) {
  
  _axe = &axe;
  _leds = &leds;

  _leds->dim(Leds::MODE_TUNER_LED);

  _layoutSetup = new LayoutSetup(_axe, this, _leds, &screen);

  //FIXME: nextLayout() crashes if all are enabled... :/

  _layouts[ScenesAndMidi] = new LayoutScenesAndMidi(_axe, this, _leds, &screen);
  _layouts[Presets] = new LayoutPresets(_axe, this, _leds, &screen);

  _layoutType = ScenesAndMidi;

  _mux.setPins(MUX0_PIN0, MUX0_PIN1, MUX0_PIN2, MUX0_PIN3);

  for (byte i=0; i<NUM_BUTTONS; i++) {
    pinMode(MUX0_SIG_PIN, INPUT_PULLUP);
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
    if (getLayout()->read(i, _buttons[i])) {
      changed = true;
    }
  }
  return changed;
}

LayoutInterface* InputManager::getLayout() { 
  if (_setupMode) {
    return _layoutSetup;
  } else {
    return _layouts[_layoutType]; 
  }
}

void InputManager::setLayoutType(LayoutType layout) { 
// Serial.printf("InputManager::setLayoutType(%d -> ", _layoutType);  
  _layoutType = layout; 
// Serial.printf("%d)\n", _layoutType);  
	getLayout()->reset();
  callLayoutChangeCallback(); 
}

void InputManager::callLayoutChangeCallback() {
  if (NULL != _layoutChangeCallback) {
    (_layoutChangeCallback)(getLayout());
  }
}

void InputManager::nextLayout() {
  byte max = arraySize(_layouts);
// Serial.println("\nInputManager::nextLayout()");
// Serial.printf("CURRENT: %d\n", _layoutType);
  byte next = (_layoutType + 1) % max;
// Serial.printf("Trying %d...\n", next);      
  while (nullptr == _layouts[next]) {
    next = (next + 1) % max;
// Serial.printf("Trying %d...\n", next);    
  }
// Serial.printf("Setting to %d\n", next);  
  setLayoutType( static_cast<LayoutType>(next) );
// Serial.printf("NEW: %s\n\n", getLayout()->getName());
}

