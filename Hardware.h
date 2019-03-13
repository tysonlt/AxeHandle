#pragma once

#include <Arduino.h>

#define DEBUG
#define P Serial.print
#define PL Serial.println

#define NUM_BUTTONS 10

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

#define MUX0_PIN0 2
#define MUX0_PIN1 3
#define MUX0_PIN2 4
#define MUX0_PIN3 5
#define MUX0_SIG_PIN 6

#define DEFAULT_DEBOUNCE 100

#define BUTTON_SHORT_HOLD_MILLIS 200
#define BUTTON_HOLD_MILLIS 500
#define BUTTON_LONG_HOLD_MILLIS 900

#define TAP_TEMPO_LED_DURATION 100

extern byte BUTTON_DEBOUNCE[NUM_BUTTONS];

extern bool BUTTON_INVERTED[NUM_BUTTONS];

extern bool BUTTON_PULLUP[NUM_BUTTONS];