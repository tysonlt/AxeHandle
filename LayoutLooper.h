#pragma once

#include <SC_Button.h>
#include "LayoutInterface.h"

class LayoutLooper : public LayoutInterface {

  friend class InputManager;

  public:

		const char* getName() { return "Looper"; }

  protected:

    enum Buttons {
      PresetUp,       //0
      SceneUp,        //1
      Undo_HalfSpeed, //2
      Once_Reverse,   //3
      ModeTuner,      //4
      PresetDown,     //5
      SceneDown,      //6
      Record,         //7
      Play,           //8
      Tap             //9
    };

    using LayoutInterface::LayoutInterface;

		void reset() {
			_leds->dimAll();
		}

    virtual bool readButton(const byte index, Button& button) {

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

        case SceneUp:
          if (PRESS) {
            _axe->sendSceneIncrement();
            return true;
          }
          break;

        case SceneDown:
          if (PRESS) {
            _axe->sendSceneDecrement();
            return true;
          }
          break;

        case Record:
          if (PRESS) {
            _axe->getLooper().record();
            return true;
          }
          break;

        case Play:
          if (PRESS) {
            _axe->getLooper().play();
            return true;
          }
          break;

        case Undo_HalfSpeed:
          if (HOLD) {
            _axe->getLooper().halfSpeed();
            return true;
          } else if (RELEASE) {
            _axe->getLooper().undo();
            return true;
          }
          break;

        case Once_Reverse:
          if (HOLD) {
            _axe->getLooper().reverse();
            return true;
          } else if (RELEASE) {
            _axe->getLooper().once();
            return true;
          }
          break;

      };
      return false;
    }

};