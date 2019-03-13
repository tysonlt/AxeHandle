#pragma once

#include "LayoutInterface.h"
#include <SC_Button.h>

class LayoutScenes : public LayoutInterface {

  friend class InputManager;

public:
  const char *getName() { return "Scenes"; }

  void reset() { turnOnSceneLed(_axe->getCurrentPreset().getSceneNumber()); }

protected:
  enum Buttons {
    Scene5,    // 0
    Scene6,    // 1
    Scene7,    // 2
    Scene8,    // 3
    ModeTuner, // 4
    Scene1,    // 5
    Scene2,    // 6
    Scene3,    // 7
    Scene4,    // 8
    Tap        // 9
  };

  using LayoutInterface::LayoutInterface;

  bool readButton(const byte index, Button &button) {

    if (button.wasPressed()) {
      SceneNumber scene = indexToScene(index);
      _axe->sendSceneChange(scene);
      turnOnSceneLed(scene);
      return true;
    }

    return false;
  }

  void turnOnSceneLed(SceneNumber scene) {
    _leds->dimAll();
    _leds->on(sceneToIndex(scene));
  }

  byte sceneToIndex(SceneNumber scene) {
    return scene < 5 ? scene + 4 : scene - 5;
  }

  SceneNumber indexToScene(byte index) {
    return index < ModeTuner ? index + 5 : index - 4;
  }
};
