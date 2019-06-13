#include "Leds.h"
#include "Hardware.h"

#define SHIFTPWM_NOSPI
static const int ShiftPWM_latchPin = SHIFTPWM_LATCH_PIN;
static const int ShiftPWM_dataPin = SHIFTPWM_DATA_PIN;
static const int ShiftPWM_clockPin = SHIFTPWM_CLOCK_PIN;
static const bool ShiftPWM_invertOutputs = ShiftPWM_INVERT_OUTPUT; 
static const bool ShiftPWM_balanceLoad = ShiftPWM_BALANCE_LOAD;
#include <ShiftPWM.h>   

void Leds::init(const byte numLeds, const byte numChips) {
  _numLeds = numLeds;
  _numChips = numChips;
  ShiftPWM.SetAmountOfRegisters(_numChips);
  ShiftPWM.Start(ShiftPWM_PWM_FREQUENCY, LED_ON);
  chase();
}

void Leds::on(const byte number) {
  dim(number, LED_ON);
}

void Leds::off(const byte number) {
  dim(number, LED_OFF);
}

void Leds::dim(const byte number, const byte brightness) {
  ShiftPWM.SetOne(number, brightness);
}

void Leds::dimAll(const byte brightness) {
  ShiftPWM.SetAll(brightness);
}

void Leds::clear() {
  ShiftPWM.SetAll(0);
}

void Leds::chase() {
  ShiftPWM.SetAll(LED_ON);
}