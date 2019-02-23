#include <AxeFxControl.h>
#include "InputManager.h"
#include "Screen.h"

AxeSystem axe;
Screen screen;
InputManager input;

void setup() {

  screen.init();

  #ifdef DEBUG
  Serial.begin(115200);
  #endif
  
  axe.begin(Serial1);
  axe.registerPresetChangeCallback(onPresetChange);
  axe.registerSystemChangeCallback(onSystemChange);
  axe.registerTunerStatusCallback(onTunerStatus);
  axe.registerTunerDataCallback(onTunerData);
  axe.enableRefresh();
  axe.refresh(true);

  input.init(axe);

}

void loop() {
  input.update();
  axe.update();
}

void onPresetChange(AxePreset preset) {
  screen.displayPreset(preset);
}

void onSystemChange() {
  screen.displayTempo(axe.getTempo());
  screen.displayFirmwareVersion(axe.getFirmwareVersion());
}

void onTunerStatus(bool connected) {
  screen.setTunerMode(connected);
}

void onTunerData(const char *note, const byte string, const byte fineTune) {
  screen.displayTunerData(note, string, fineTune);
}