#pragma once

#include <Arduino.h>

class Leds {

  public:

    static const byte TAP_TEMPO_LED = 4;
    static const byte NUM_LEDS = 10;

    void init();

    void on(const byte number);
    void off(const byte number);
    void dim(const byte number, const byte brightness = 16);
    void dimAll(const byte brightness = 16);
    void chase();

    void clear();
    void update();
    
  private:

    static const unsigned LED_ON = 4095;
    static const unsigned LED_OFF = 0;

    void doChase(const byte number);

    bool _dirty = false;

};