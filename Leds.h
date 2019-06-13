#pragma once

#include <Arduino.h>

class Leds {

public:
  static const byte MODE_TUNER_LED = 4;
  static const byte TAP_TEMPO_LED = 9;
  static const unsigned LED_ON = 255;
  static const unsigned LED_OFF = 0;
  static const unsigned LED_DIM = 16;

  void init(const byte numLeds, const byte numChips);

  void on(const byte number);
  void off(const byte number);
  void dim(const byte number, const byte brightness = LED_DIM);
  void dimAll(const byte brightness = LED_DIM);
  void chase();

  void clear();

  byte getLedCount() { return _numLeds; }
  byte getChipCount() { return _numChips; }

private:

  byte _numLeds;
  byte _numChips;

};