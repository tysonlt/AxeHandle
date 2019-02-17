#include "Hardware.h"

byte BUTTON_DEBOUNCE[NUM_BUTTONS] = {
  25,
  25,
  25,
  25,
  25,
  25,
  25,
  25,
  25,
  25
};

bool BUTTON_INVERTED[NUM_BUTTONS] = {
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false
};

bool BUTTON_PULLUP[NUM_BUTTONS] = {
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false
};