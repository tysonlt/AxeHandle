#pragma once

#include <Arduino.h>
#include <AxeFxControl.h>
#include <SC_Multiplexer.h>
#include <SC_Button.h> 
#include "Hardware.h"
#include "Leds.h"

enum LayoutType {
  PedalsAndScenes,
  Presets,
  Pedals,
  Scenes,
	// Midi,
  //Looper,
  //KitchenSink,
  __NUM_LAYOUT_TYPES
};

class LayoutInterface;
class Screen;
typedef void (*LayoutChangeCallback)(LayoutInterface *layout);

class InputManager {

  public:

    void init(AxeSystem& axe, Leds& leds, Screen& screen);
    bool update();
    void nextLayout();
    
    void setLayoutType(LayoutType layout);
    LayoutType getLayoutType() { return _layoutType; }
		LayoutInterface* getLayout() { return _layouts[_layoutType]; }

    void registerLayoutChangeCallback(LayoutChangeCallback func) { _layoutChangeCallback = func; }
    void callLayoutChangeCallback();

  private:

    AxeSystem *_axe = nullptr;
    Leds *_leds = nullptr;
    Button _buttons[NUM_BUTTONS];
    Multiplexer _mux;
    LayoutType _layoutType;
    LayoutInterface *_layouts[__NUM_LAYOUT_TYPES];
    LayoutChangeCallback _layoutChangeCallback = NULL;

};