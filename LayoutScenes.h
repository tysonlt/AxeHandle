#pragma once

#include <SC_Button.h>
#include "LayoutInterface.h"
#include "Hardware.h"

class LayoutScenes : public LayoutInterface {

  friend class InputManager;

  public:

    virtual void update() {}

  protected:

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

    using LayoutInterface::LayoutInterface;

    virtual bool readButton(byte index, Button& button) {

      if (processStandardButtons(index, button)) {
        return true;
      }

      switch(index) {

        case PresetUp:
          if (PRESS) {
            _axe->sendPresetIncrement();
            return true;
          }
          break;
        
        case PresetDown:
          if (PRESS) {
            _axe->sendPresetDecrement();
            return true;
          }
          break;

        case Scene1:
          if (PRESS) {
            _axe->sendSceneChange(1);
            return true;
          }
          break;

        case Scene2:
          if (PRESS) {
            _axe->sendSceneChange(2);
            return true;
          }
          break;

        case Scene3_8:
          if (HOLD) {
            _axe->sendSceneChange(8);
            return true;
          } else if (RELEASE) {
            _axe->sendSceneChange(3);
            return true;
          }
          break;

        case Scene4:
          if (PRESS) {
            _axe->sendSceneChange(4);
            return true;
          }
          break;

        case Scene5:
          if (PRESS) {
            _axe->sendSceneChange(5);
            return true;
          }
          break;

        case Scene6_7:
          if (HOLD) {
            _axe->sendSceneChange(7);
            return true;
          } else if (RELEASE) {
            _axe->sendSceneChange(6);
            return true;
          }
          break;

      };

      return false;

    }

};
