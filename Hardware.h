#pragma once

#include <Arduino.h>

#define NUM_BUTTONS 10

#define MUX0_PIN0 4
#define MUX0_PIN1 5
#define MUX0_PIN2 6
#define MUX0_PIN3 7
#define MUX0_SIG_PIN 27

#define BUTTON_HOLD_MILLIS 800

extern byte BUTTON_DEBOUNCE[NUM_BUTTONS];

extern bool BUTTON_INVERTED[NUM_BUTTONS];

extern bool BUTTON_PULLUP[NUM_BUTTONS];