#pragma once

#include "Hardware.h"
#include "LayoutInterface.h"
#include <SC_Button.h>
#include <EEPROMEx.h>

//commands
enum Command {
  PRESET_DOWN,
  PRESET_UP,
  SCENE_DOWN,
  SCENE_UP,
  SCENE_CHANGE,
  EFFECT_TOGGLE,
  MIDI_PC,
  MIDI_CC,
  __COMMAND_COUNT
};

//button mode
enum ButtonMode {
  ON_PRESS,
  ON_RELEASE,
  ON_HOLD_SHORT,
  ON_HOLD_MEDIUM,
  ON_HOLD_LONG,
  __BUTTON_MODE_COUNT
};

struct ButtonAssignment {
  Command command;
  ButtonMode mode;
  byte data1; //effect id, scene number, midi pc/cc 
  byte data2; //cc value
};

class LayoutSetup;
class LayoutUser : public LayoutInterface {

  friend class InputManager;
  friend class LayoutSetup;

public:
  const char *getName() { return "User"; }

protected:

  LayoutUser(AxeSystem *axe, InputManager *input, Leds *leds, Screen *screen) : 
  LayoutInterface(axe, input, leds, screen) {

    //TODO: TEST load button assignments from eeprom
    EEPROM.readBlock(EEPROM_ADDRESS, _buttonAssignments, ELEMENT_COUNT);

  }

  void reset() { _leds->dimAll(); }

  bool readButton(const byte index, Button &button) {

    ButtonAssignment assignment = _buttonAssignments[index];

    if (checkButton(assignment.mode, index, button)) {
    
      switch (assignment.command) {

        case PRESET_DOWN:
          _axe->sendPresetDecrement();
          return true;

        case PRESET_UP:
          _axe->sendPresetIncrement();
          return true;
        
        case SCENE_DOWN:
          _axe->sendSceneDecrement();
          return true;
        
        case SCENE_UP:
          _axe->sendSceneIncrement();
          return true;
        
        case SCENE_CHANGE:
          _axe->sendSceneChange(assignment.data1);
          return true;
        
        case EFFECT_TOGGLE:
          if (AxeEffect *effect = _axe->getCurrentPreset().getEffectById(assignment.data1)) {
            effect->toggle();
            return true;
          }
          break;      
        
        case MIDI_PC:
          _axe->sendProgramChange(assignment.data1, _axe->getMidiChannel());
          return true;

        case MIDI_CC:
          _axe->sendControlChange(assignment.data1, assignment.data2, _axe->getMidiChannel());
          return true;

        default:
          return false;

      }
    
    }

    return false;

  }

  bool checkButton(ButtonMode mode, const byte index, Button& button) {
    switch (mode) {
      case ON_RELEASE:
        return RELEASE;
      case ON_HOLD_SHORT:
        return HOLD_SHORT;
      case ON_HOLD_MEDIUM:
        return HOLD;
      case ON_HOLD_LONG:
        return HOLD_LONG;
      default: 
        return PRESS;
    }
  }

  static const int EEPROM_ADDRESS = 0;
  static const int ELEMENT_COUNT = 9; //index 4 (MODE) is ignored

  ButtonAssignment _buttonAssignments[ELEMENT_COUNT];

};
