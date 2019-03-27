#pragma once

#include <Arduino.h>

class Leds {

public:
  static const byte MODE_TUNER_LED = 4;
  static const byte TAP_TEMPO_LED = 9;
  static const byte NUM_LEDS = 10;

  void init();

  void on(const byte number);
  void off(const byte number);
  void dim(const byte number, const byte brightness = LED_DIM);
  void dimAll(const byte brightness = LED_DIM);
  void chase();

  void clear();

private:
  static const unsigned LED_ON = 255;
  static const unsigned LED_OFF = 0;
  static const unsigned LED_DIM = 16;

};