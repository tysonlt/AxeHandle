#pragma once

#include <Arduino.h>

#define DEBUG
#define P_ Serial.print
#define PL_ Serial.println

#define AXE_REFRESH_RATE 3000

#define NUM_BUTTONS 10
#define NUM_LEDS 10
#define NUM_PWM_CHIPS 2

#define TFT_DC 10
#define TFT_CS 9
#define TFT_RST 8

#define SHIFTPWM_CLOCK_PIN 7
#define SHIFTPWM_LATCH_PIN 6
#define SHIFTPWM_DATA_PIN 5
#define ShiftPWM_INVERT_OUTPUT false
#define ShiftPWM_BALANCE_LOAD false
#define ShiftPWM_PWM_FREQUENCY 75

#define MUX0_PIN0 15
#define MUX0_PIN1 16
#define MUX0_PIN2 17
#define MUX0_PIN3 18
#define MUX0_SIG_PIN 19

#define BUTTON1_PIN 14

#define DEFAULT_DEBOUNCE 25

#define BUTTON_SHORT_HOLD_MILLIS 350
#define BUTTON_HOLD_MILLIS 500
#define BUTTON_LONG_HOLD_MILLIS 1500

#define TAP_TEMPO_LED_DURATION 100

extern byte BUTTON_DEBOUNCE[NUM_BUTTONS];
extern bool BUTTON_INVERTED[NUM_BUTTONS];
extern bool BUTTON_PULLUP[NUM_BUTTONS];