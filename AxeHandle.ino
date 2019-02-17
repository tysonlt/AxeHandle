#include <AxeFxControl.h>
#include "InputManager.h"
#include "Screen.h"

AxeSystem axe;
Screen screen;
InputManager input;

void setup() {

  Serial.begin(115200);
  
  screen.init();
  axe.begin(Serial1);
  input.init(axe);
  
  axe.registerPresetChangeCallback(onPresetChange);
  axe.registerSystemChangeCallback(onSystemChange);
  
  axe.enableRefresh(500);
  axe.requestPresetDetails();
  axe.requestTempo();

}

void loop() {
  input.update();
  axe.update();
}

void onPresetChange(AxePreset preset) {
  screen.printPreset(preset);
}

void onSystemChange() {
  screen.printTempo(axe.getTempo());
}