#pragma once

#include <SC_Button.h>
#include "ControlScheme.h"

class ControlSchemeLooper : public ControlScheme {

  enum Buttons {
    PresetUp,       //0
    SceneUp,        //1
    Undo_HalfSpeed, //2
    Once_Reverse,   //3
    ModeTuner,      //4
    PresetDown,     //5
    SceneDown,       //6
    Record,         //7
    Play,           //8
    Tap             //9
  };

  public:

    using ControlScheme::ControlScheme;

    virtual bool readButton(byte index, Button& button) {
      switch(index) {

        case Tap:
          if (button.wasPressed()) {
            _axe->sendTap();
            return true;
          }
          break;
        
        case PresetUp:
          if (button.wasPressed()) {
            _axe->sendPresetIncrement();
            return true;
          }
          break;

        case PresetDown:
          if (button.wasPressed()) {
            _axe->sendPresetDecrement();
            return true;
          }
          break;

        case SceneUp:
          if (button.wasPressed()) {
            _axe->sendSceneIncrement();
            return true;
          }
          break;

        case SceneDown:
          if (button.wasPressed()) {
            _axe->sendSceneDecrement();
            return true;
          }
          break;

        case Record:
          if (button.wasPressed()) {
            _axe->getLooper().record();
            return true;
          }
          break;

        case Play:
          if (button.wasPressed()) {
            _axe->getLooper().play();
            return true;
          }
          break;

        case Undo_HalfSpeed:
          if (button.pressedFor(BUTTON_HOLD_MILLIS)) {
            _axe->getLooper().halfSpeed();
            return true;
          } else if (button.wasReleased()) {
            _axe->getLooper().undo();
            return true;
          }
          break;

        case Once_Reverse:
          if (button.pressedFor(BUTTON_HOLD_MILLIS)) {
            _axe->getLooper().reverse();
            return true;
          } else if (button.wasReleased()) {
            _axe->getLooper().once();
            return true;
          }
          break;

        case ModeTuner:
          if (button.pressedFor(BUTTON_HOLD_MILLIS)) {
            _input->nextControlScheme();
            return true;
          } else if (button.wasReleased()) {
            _axe->toggleTuner();
            return true;
          }
          break;
      };
      return false;
    }

};