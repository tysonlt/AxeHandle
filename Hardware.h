#pragma once

#include <Arduino.h>

#define DEBUG
#define P Serial.print
#define PL Serial.println

#define NUM_BUTTONS 10

#define MUX0_PIN0 4
#define MUX0_PIN1 5
#define MUX0_PIN2 6
#define MUX0_PIN3 7
#define MUX0_SIG_PIN 27

#define DEFAULT_DEBOUNCE 100

#define BUTTON_SHORT_HOLD_MILLIS 200
#define BUTTON_HOLD_MILLIS 400
#define BUTTON_LONG_HOLD_MILLIS 900

#define TAP_TEMPO_LED_DURATION 100

extern byte BUTTON_DEBOUNCE[NUM_BUTTONS];

extern bool BUTTON_INVERTED[NUM_BUTTONS];

extern bool BUTTON_PULLUP[NUM_BUTTONS];