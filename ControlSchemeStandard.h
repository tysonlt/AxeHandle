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

    using ControlScheme::ControlScheme;

    virtual bool readButton(byte index, Button& button) {

      switch(index) {
        
        case Tap:
          if (PRESS) {
Serial.println("Tap");            
            _axe->sendTap();
            return true;
          }
          break;

        case PresetUp:
          if (PRESS) {
Serial.println("PresetUp");            
            _axe->sendPresetIncrement();
            return true;
          }
          break;
        
        case PresetDown:
          if (PRESS) {
Serial.println("PresetDown");            
            _axe->sendPresetDecrement();
            return true;
          }
          break;

        case ModeTuner:
          if (HOLD) {
Serial.println("Mode");            
            _input->nextControlScheme();
            return true;
          } else if (RELEASE) {
            _axe->toggleTuner();
Serial.println("Tuner");            
            return true;
          }
          break;

        case Scene1:
          if (PRESS) {
Serial.println("Scene1");            
            _axe->sendSceneChange(1);
            return true;
          }
          break;

        case Scene2:
          if (PRESS) {
Serial.println("Scene2");            
            _axe->sendSceneChange(2);
            return true;
          }
          break;

        case Scene3_8:
          if (HOLD) {
Serial.println("Scene8");            
            _axe->sendSceneChange(8);
            return true;
          } else if (RELEASE) {
Serial.println("Scene3");            
            _axe->sendSceneChange(3);
            return true;
          }
          break;

        case Scene4:
          if (PRESS) {
Serial.println("Scene4");            
            _axe->sendSceneChange(4);
            return true;
          }
          break;

        case Scene5:
          if (PRESS) {
Serial.println("Scene5");            
            _axe->sendSceneChange(5);
            return true;
          }
          break;

        case Scene6_7:
          if (HOLD) {
Serial.println("Scene7");            
            _axe->sendSceneChange(7);
            return true;
          } else if (RELEASE) {
Serial.println("Scene6");            
            _axe->sendSceneChange(6);
            return true;
          }
          break;

      };

      return false;

    }

};
