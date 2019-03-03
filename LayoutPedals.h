#pragma once

#include <SC_Button.h>
#include "LayoutInterface.h"
#include "Hardware.h"

class LayoutPedals : public LayoutInterface {

  friend class InputManager;

  public:

    virtual void update() {
      _axe->isEffectEnabled(ID_FUZZ1)   ? _leds->on(Drive1) : _leds->dim(Drive1);
      _axe->isEffectEnabled(ID_FUZZ2)   ? _leds->on(Drive2) : _leds->dim(Drive2);
      _axe->isEffectEnabled(ID_DELAY1)  ? _leds->on(Delay1) : _leds->dim(Delay1);
      _axe->isEffectEnabled(ID_DELAY2)  ? _leds->on(Delay2) : _leds->dim(Delay2);
      _axe->isEffectEnabled(ID_WAH1)    ? _leds->on(Wah1)   : _leds->dim(Wah1);
      _axe->isEffectEnabled(ID_CHORUS1) ? _leds->on(Mod1)   : _leds->dim(Mod1);
    }

  protected:

    enum Buttons {
      PresetSceneUp,    //0
      Drive1,           //1
      Delay1,           //2
      Wah1,             //3
      Tap,              //4
      PresetSceneDown,  //5
      Drive2,           //6
      Delay2,           //7
      Mod1,             //8
      ModeTuner         //9
    };

    using LayoutInterface::LayoutInterface;

    virtual bool readButton(byte index, Button& button) {

      if (!_initialised) {
        _initialised = true;
        setupLeds();
      }

      if (processStandardButtons(index, button)) {
        return true;
      }

      switch(index) {

        case PresetSceneUp:
          if (HOLD) {
            _axe->sendPresetIncrement();
            return true;
          } else if (RELEASE) {
            _axe->sendSceneIncrement();
            return true;
          }
          break;
        
        case PresetSceneDown:
          if (HOLD) {
            _axe->sendPresetDecrement();
            return true;
          } else if (RELEASE) {
            _axe->sendSceneDecrement();
            return true;
          }
          break;

        case Drive1:
          if (PRESS) {
            _axe->toggleEffect(ID_FUZZ1);
            return true;
          }
          break;

        case Delay1:
          if (PRESS) {
            _axe->toggleEffect(ID_DELAY1);
            return true;
          }
          break;

        case Wah1:
          if (PRESS) {
            _axe->toggleEffect(ID_WAH1);
            return true;
          }
          break;

        case Drive2:
          if (PRESS) {
            _axe->toggleEffect(ID_FUZZ2);
            return true;
          }
          break;
          
        case Delay2:
          if (PRESS) {
            _axe->toggleEffect(ID_DELAY2);
            return true;
          }
          break;

        case Mod1:
          if (PRESS) {
            _axe->toggleEffect(ID_CHORUS1);
            return true;
          }
          break;

      };

      return false;

    }

  private:

    void setupLeds() {
      _leds->dimAll();
      _leds->on(PresetSceneUp);
      _leds->on(PresetSceneDown);
    }

/*
      if (_axe->isEffectEnabled(ID_FUZZ1)) 
        _leds->on(Drive1);

      if (_axe->isEffectEnabled(ID_FUZZ2)) 
        _leds->on(Drive2);

      if (_axe->isEffectEnabled(ID_DELAY1)) 
        _leds->on(Delay1);

      if (_axe->isEffectEnabled(ID_DELAY2)) 
        _leds->on(Delay2);

      if (_axe->isEffectEnabled(ID_WAH1)) 
        _leds->on(Wah1);

      if (_axe->isEffectEnabled(ID_CHORUS1)) 
        _leds->on(Mod1);
*/

    bool _initialised = false;

};
