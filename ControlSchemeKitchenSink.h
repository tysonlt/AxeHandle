#pragma once

#include <SC_Button.h>
#include "ControlScheme.h"
#include "Hardware.h"

class ControlSchemeKitchenSink : public ControlScheme {

  enum Buttons {
    PresetUp,       //0
    Scene3_7,       //1
    Scene4_8,       //2
    Record_Undo,    //3
    ModeTuner,      //4
    PresetDown,     //5
    Scene1_5,       //6
    Scene2_6,       //7
    Play_Once,      //8
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
            _axe->sendPresetIncrement();
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

        case Scene1_5:
          if (button.pressedFor(BUTTON_HOLD_MILLIS)) {
            _axe->sendSceneChange(5);
            return true;
          } else if (button.wasReleased()) {
            _axe->sendSceneChange(1);
            return true;
          }
          break;

        case Scene2_6:
          if (button.pressedFor(BUTTON_HOLD_MILLIS)) {
            _axe->sendSceneChange(6);
            return true;
          } else if (button.wasReleased()) {
            _axe->sendSceneChange(2);
            return true;
          }
          break;

        case Scene3_7:
          if (button.pressedFor(BUTTON_HOLD_MILLIS)) {
            _axe->sendSceneChange(7);
            return true;
          } else if (button.wasReleased()) {
            _axe->sendSceneChange(3);
            return true;
          }
          break;

        case Scene4_8:
          if (button.pressedFor(BUTTON_HOLD_MILLIS)) {
            _axe->sendSceneChange(8);
            return true;
          } else if (button.wasReleased()) {
            _axe->sendSceneChange(4);
            return true;
          }
          break;

        case Record_Undo:
          if (button.pressedFor(BUTTON_HOLD_MILLIS)) {
            _axe->getLooper().undo();
            return true;
          } else if (button.wasReleased()) {
            _axe->getLooper().record();
            return true;
          }
          break;

        case Play_Once:
          if (button.pressedFor(BUTTON_HOLD_MILLIS)) {
            _axe->getLooper().once();
            return true;
          } else if (button.wasReleased()) {
            _axe->getLooper().play();
            return true;
          }
          break;

      };

      return false;

    }

};