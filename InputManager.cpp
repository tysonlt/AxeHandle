#include "InputManager.h"
#include "LayoutLooper.h"
#include "LayoutMidi.h"
#include "LayoutPedals.h"
#include "LayoutPedalsAndScenes.h"
#include "LayoutPresets.h"
#include "LayoutScenes.h"
#include "LayoutScenesAndMidi.h"
#include "LayoutSetup.h"
#include "LayoutUser.h"
#include <SC_Button.h>
#include <SC_Multiplexer.h>

//if any button is pressed during startup (when this is called), go into setup mode
void InputManager::checkSetupMode() {

#ifdef TESTING_SETUP
    _setupMode = true;
    while (true)
        update();
#endif

    for (byte i = 0; i < NUM_BUTTONS; i++) {
        if (_buttons[i].isPressed()) {
            _setupMode = true;
            while (true) {
                update();
            }
        }
    }
}

void InputManager::init(AxeSystem &axe, Leds &leds, Screen &screen) {

    _axe = &axe;
    _leds = &leds;

    _leds->dim(Leds::MODE_TUNER_LED);
    _layoutSetup = new LayoutSetup(_axe, this, _leds, &screen);

    //FIXME: nextLayout() crashes if all are enabled... :/
    //add more layouts here if desired, I only use these two
    _layouts[ScenesAndMidi] = new LayoutScenesAndMidi(_axe, this, _leds, &screen);
    _layouts[Presets] = new LayoutPresets(_axe, this, _leds, &screen);

    //set default layout
    _layoutType = ScenesAndMidi;

    //setup multiplexer and init buttons
    _mux.setPins(MUX0_PIN0, MUX0_PIN1, MUX0_PIN2, MUX0_PIN3);
    for (byte i = 0; i < NUM_BUTTONS; i++) {
        pinMode(MUX0_SIG_PIN, INPUT_PULLUP);
        _buttons[i].setPin(MUX0_SIG_PIN);
        _buttons[i].setMultiplexer(&_mux, i);
        _buttons[i].setDebounceTime(BUTTON_DEBOUNCE[i]);
        _buttons[i].setInverted(BUTTON_INVERTED[i]);
        _buttons[i].setPullupEnable(BUTTON_PULLUP[i]);
        _buttons[i].begin();
    }
}

//reads buttons and returns true if any have changed
bool InputManager::update() {
    bool changed = false;
    for (byte i = 0; i < NUM_BUTTONS; i++) {
        _buttons[i].read();
        if (getLayout()->read(i, _buttons[i])) {
            changed = true;
        }
    }
    return changed;
}

//get current layout
LayoutInterface *InputManager::getLayout() {
    if (_setupMode) {
        return _layoutSetup;
    } else {
        return _layouts[_layoutType];
    }
}

//set new layout
void InputManager::setLayoutType(LayoutType layout) {
    _layoutType = layout;
    getLayout()->reset();
    callLayoutChangeCallback();
}

//tell layout it has been changed
void InputManager::callLayoutChangeCallback() {
    if (NULL != _layoutChangeCallback) {
        (_layoutChangeCallback)(getLayout());
    }
}

//cycle through layouts and set next layout
void InputManager::nextLayout() {
    byte max = arraySize(_layouts);
    byte next = (_layoutType + 1) % max;
    
    while (nullptr == _layouts[next]) {
        next = (next + 1) % max;
    }
    
    setLayoutType(static_cast<LayoutType>(next));
}
