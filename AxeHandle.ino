#include <AxeFxControl.h>
#include <Timer.h>
#include "Hardware.h"
#include "InputManager.h"
#include "Leds.h"
#include "Screen.h"

AxeSystem axe;
Screen screen;
InputManager input;
Timer timer;
Leds leds;

void setup() {

  screen.init();

  leds.init();

  #ifdef DEBUG
  Serial.begin(115200);
  #endif
  
  axe.begin(Serial1);
  axe.registerPresetChangeCallback(onPresetChange);
  axe.registerSystemChangeCallback(onSystemChange);
  axe.registerTunerStatusCallback(onTunerStatus);
  axe.registerTunerDataCallback(onTunerData);
  axe.registerTapTempoCallback(onTapTempo);
	axe.registerEffectFilterCallback(onEffectFilter);
  axe.enableRefresh();
  axe.refresh(true);

  input.registerLayoutChangeCallback(onLayoutChange);
  input.init(axe, leds, screen);

}

void loop() {
  timer.update();
  input.update();
  axe.update();
  leds.update();
}

void onTapTempo() {
  leds.on(Leds::TAP_TEMPO_LED);
  timer.after(TAP_TEMPO_LED_DURATION, turnOffTapTempoLed);
}

void turnOffTapTempoLed() {
  leds.off(Leds::TAP_TEMPO_LED);
}

bool onEffectFilter(const PresetNumber number, AxeEffect effect) {
	return input.getLayout()->filterEffect(number, effect);
}

void onLayoutChange(LayoutInterface *layout) {
  screen.displayLayout(layout);
}

void onPresetChange(AxePreset preset) {
  input.getLayout()->reset();
	screen.displayPreset(preset);
  screen.displayLayout(input.getLayout());
}

void onSystemChange() {
  screen.displayTempo(axe.getTempo());
  screen.displayFirmwareVersion(axe.getFirmwareVersion());
  screen.displayLayout(input.getLayout());
}

void onTunerStatus(bool engaged) {
	engaged ? leds.on(Leds::MODE_TUNER_LED) : leds.dim(Leds::MODE_TUNER_LED);
  screen.setTunerMode(engaged);
}

void onTunerData(const char *note, const byte string, const byte fineTune) {
  screen.displayTunerData(note, string, fineTune);
}