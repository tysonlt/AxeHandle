#pragma once

#include "Hardware.h"
#include "LayoutInterface.h"
#include <SC_Button.h>

class LayoutScenesAndMidi : public LayoutInterface {

  friend class InputManager;

public:
  const char *getName() { return "Scene/MIDI"; }

  void reset() { 
    _leds->dimAll();
    turnOnSceneLed( _axe->getCurrentPreset().getSceneNumber() );
  }

protected:
  enum Buttons {
    Scene1,    // 0
    Scene2,    // 1
    Scene3,    // 2
    Scene4,    // 3
    ModeTuner, // 4
    Midi1,     // 5
    Midi2,     // 6
    Midi3,     // 7
    Midi4,     // 8
    Tap        // 9
  };

  using LayoutInterface::LayoutInterface;

  bool readButton(const byte index, Button &button) {

    if (index < ModeTuner) {
      return processScene(index, button);
    } else {
      return processMidi(index, button);
    }

    return false;

  }

private:

  bool processScene(const byte index, Button &button) {

    byte scene = 0;

    //FIXME: sometimes goes to release instead on HOLD_SHORT
    if (HOLD) {
      scene = index + 5;
    } else if (RELEASE) {
      scene = index + 1;
    }

    if (scene > 0) {
      _axe->sendSceneChange(scene);
      turnOnSceneLed(scene);
      return true;
    }

    return false;

  }

  bool processMidi(const byte index, Button &button) {

    byte controller = 0, value = 0;

    if (HOLD_SHORT) {
      controller = index;  
    } else if (RELEASE) {
      controller = index - 4;  
    }

    if (controller > 0) {
      _axe->sendControlChange(controller, value, _axe->getMidiChannel());
    }

    //no need to refresh
    return false;

  }

  void turnOnSceneLed(SceneNumber scene) {
    _leds->dim(0);
    _leds->dim(1);
    _leds->dim(2);
    _leds->dim(3);
    if (scene <= 4) {
      _leds->on(scene - 1);
    }
  }

};