#pragma once

#include <Arduino.h>
#include <AxeFxControl.h>
#include <SC_Multiplexer.h>
#include <SC_Button.h> 
#include "Hardware.h"
#include "Leds.h"

enum LayoutType {
  Pedals,
  Scenes,
  Looper,
  KitchenSink,
  __NUM_LAYOUT_TYPES
};

typedef void (*LayoutChangeCallback)(const LayoutType layout);

class LayoutInterface;
class InputManager {

  public:

    void init(AxeSystem& axe, Leds& leds);
    bool update();
    void updateLeds();
    void nextLayout();
    
    void setLayout(LayoutType layout);
    LayoutType getLayout() { return _layoutType; }

    void registerLayoutChangeCallback(LayoutChangeCallback func) { _layoutChangeCallback = func; }
    void callLayoutChangeCallback();

  private:

    LayoutInterface* layout() {
      return _layouts[_layoutType];
    }

    AxeSystem *_axe = nullptr;
    Leds *_leds = nullptr;
    Button _buttons[NUM_BUTTONS];
    Multiplexer _mux;
    LayoutType _layoutType;
    LayoutInterface *_layouts[__NUM_LAYOUT_TYPES];
    LayoutChangeCallback _layoutChangeCallback = NULL;

};