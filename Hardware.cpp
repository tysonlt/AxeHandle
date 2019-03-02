#include "Hardware.h"

byte BUTTON_DEBOUNCE[NUM_BUTTONS] = {
  DEFAULT_DEBOUNCE,
  DEFAULT_DEBOUNCE,
  DEFAULT_DEBOUNCE,
  DEFAULT_DEBOUNCE,
  DEFAULT_DEBOUNCE,
  DEFAULT_DEBOUNCE,
  DEFAULT_DEBOUNCE,
  DEFAULT_DEBOUNCE,
  DEFAULT_DEBOUNCE,
  DEFAULT_DEBOUNCE
};

bool BUTTON_INVERTED[NUM_BUTTONS] = {
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true
};

bool BUTTON_PULLUP[NUM_BUTTONS] = {
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true
};