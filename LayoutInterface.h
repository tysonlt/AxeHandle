#pragma once

#include "Hardware.h"
#include "InputManager.h"
#include "Leds.h"
#include "Screen.h"
#include <AxeFxControl.h>
#include <SC_Button.h>

#define PRESS button.wasPressed()
#define HOLD buttonHeld(button, index, BUTTON_HOLD_MILLIS)
#define HOLD_SHORT buttonHeld(button, index, BUTTON_SHORT_HOLD_MILLIS)
#define HOLD_LONG buttonHeld(button, index, BUTTON_LONG_HOLD_MILLIS)
#define RELEASE buttonReleased(button, index)

class LayoutInterface {

public:

  virtual const char *getName() = 0;
  virtual void reset() = 0;

  //read the given button
  //note that TAP and TUNER are hard-coded
  virtual bool read(const byte index, Button &button) {
    
      if (index == BUTTON_INDEX_TAP) {
      
        if (PRESS) {
          _axe->sendTap();
          return true;
        }
      
    } else if (index == BUTTON_INDEX_MODE_TUNER) {

        if (_axe->isTunerEngaged()) {
          if (RELEASE) {
            _axe->disableTuner();
            return true;
          }
        } else {
          if (HOLD_SHORT) {
            _input->nextLayout();
            return true;
          } else if (RELEASE) {
            _axe->enableTuner();
            return true;
          }
        }

    } else {

      return readButton(index, button);

    }

    return false;

  }

protected:
  static const byte BUTTON_INDEX_PRESET_UP = 0;
  static const byte BUTTON_INDEX_PRESET_DOWN = 5;
  static const byte BUTTON_INDEX_TAP = 9;
  static const byte BUTTON_INDEX_MODE_TUNER = 4;

  LayoutInterface(AxeSystem *axe, InputManager *input, Leds *leds, Screen *screen) {
    _axe = axe;
    _input = input;
    _leds = leds;
    _screen = screen;
    _leds->dimAll();
  }

  virtual bool readButton(const byte index, Button &button) = 0;

  bool buttonHeld(Button &button, const byte index, const millis_t millis = BUTTON_HOLD_MILLIS); 
  bool buttonReleased(Button &button, const byte index);

  AxeSystem *_axe = nullptr;
  InputManager *_input = nullptr;
  Leds *_leds = nullptr;
  Screen *_screen = nullptr;

};