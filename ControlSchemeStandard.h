#pragma once

#include <SC_Button.h>
#include "ControlScheme.h"
#include "Hardware.h"

class ControlSchemeStandard : public ControlScheme {

  enum Buttons {
    PresetUp,   //0
    Scene4,     //1
    Scene5,     //2
    Scene6_7,   //3
    ModeTuner,  //4
    PresetDown, //5
    Scene1,     //6
    Scene2,     //7
    Scene3_8,   //8
    Tap         //9
  };

  public:

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

        case Scene1:
          if (button.wasPressed()) {
            _axe->sendSceneChange(1);
            return true;
          }
          break;

        case Scene2:
          if (button.wasPressed()) {
            _axe->sendSceneChange(2);
            return true;
          }
          break;

        case Scene3_8:
          if (button.pressedFor(BUTTON_HOLD_MILLIS)) {
            _axe->sendSceneChange(8);
            return true;
          } else if (button.wasReleased()) {
            _axe->sendSceneChange(3);
            return true;
          }
          break;

        case Scene4:
          if (button.wasPressed()) {
            _axe->sendSceneChange(4);
            return true;
          }
          break;

        case Scene5:
          if (button.wasPressed()) {
            _axe->sendSceneChange(5);
            return true;
          }
          break;

        case Scene6_7:
          if (button.pressedFor(BUTTON_HOLD_MILLIS)) {
            _axe->sendSceneChange(7);
            return true;
          } else if (button.wasReleased()) {
            _axe->sendSceneChange(6);
            return true;
          }
          break;

      };

      return false;

    }

};