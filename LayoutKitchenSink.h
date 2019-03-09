#pragma once

#include <SC_Button.h>
#include "LayoutInterface.h"
#include "Hardware.h"

class LayoutKitchenSink : public LayoutInterface {

  friend class InputManager;

  public:

		const char* getName() { return "KitchenSink"; }

  protected:

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

    using LayoutInterface::LayoutInterface;

		void reset() {
			_leds->dimAll();
		}

    bool readButton(const byte index, Button& button) {

      switch(index) {
        
        case PresetUp:
          if (PRESS) {
            _axe->sendPresetIncrement();
            return true;
          }
          break;
        
        case PresetDown:
          if (PRESS) {
            _axe->sendPresetIncrement();
            return true;
          }
          break;

        case Scene1_5:
          if (HOLD) {
            _axe->sendSceneChange(5);
            return true;
          } else if (RELEASE) {
            _axe->sendSceneChange(1);
            return true;
          }
          break;

        case Scene2_6:
          if (HOLD) {
            _axe->sendSceneChange(6);
            return true;
          } else if (RELEASE) {
            _axe->sendSceneChange(2);
            return true;
          }
          break;

        case Scene3_7:
          if (HOLD) {
            _axe->sendSceneChange(7);
            return true;
          } else if (RELEASE) {
            _axe->sendSceneChange(3);
            return true;
          }
          break;

        case Scene4_8:
          if (HOLD) {
            _axe->sendSceneChange(8);
            return true;
          } else if (RELEASE) {
            _axe->sendSceneChange(4);
            return true;
          }
          break;

        case Record_Undo:
          if (HOLD) {
            _axe->getLooper().undo();
            return true;
          } else if (RELEASE) {
            _axe->getLooper().record();
            return true;
          }
          break;

        case Play_Once:
          if (HOLD) {
            _axe->getLooper().once();
            return true;
          } else if (RELEASE) {
            _axe->getLooper().play();
            return true;
          }
          break;

      };

      return false;

    }

};