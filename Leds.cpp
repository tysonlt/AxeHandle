#include <Tlc5940.h>
#include "Leds.h"

void Leds::init() {
  Tlc.init();
  chase();
  _dirty = false;
}

void Leds::on(const byte number) {
  Tlc.set(number, LED_ON);
  _dirty = true;
}

void Leds::off(const byte number) {
  Tlc.set(number, LED_OFF);
  _dirty = true;
}

void Leds::dim(const byte number, const byte brightness) {
  Tlc.set(number, map(brightness, 0, 255, LED_OFF, LED_ON));
  _dirty = true;
}

void Leds::dimAll(const byte brightness) {
  clear();
  for (byte i=0; i<NUM_LEDS; i++) {
    dim(i, brightness);
  }
}

void Leds::clear() {
  Tlc.clear();
  _dirty = true;
}

void Leds::update() {
  if (_dirty) {
    _dirty = false;
    Tlc.update();
  }
}

void Leds::chase() {
  
  for (int channel = 0; channel < NUM_LEDS; channel++) {
    doChase(channel);
  }

  for (int channel = NUM_LEDS; channel > 0; channel--) {
    doChase(channel);
  }

  clear();
  update();

}

void Leds::doChase(const byte channel) {
  clear();
  if (channel > 0) 
    dim(channel - 1, 64);
  on(channel);
  if (channel < Leds::NUM_LEDS) 
    dim(channel + 1, 64);
  update();
  delay(75);
}