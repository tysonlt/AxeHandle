#include "LayoutInterface.h"

bool _holdState[NUM_BUTTONS];

//remember hold state so held button doesn't fire multiple times when held down
bool LayoutInterface::buttonHeld(Button &button, const byte index, const millis_t millis) {
    if (button.pressedFor(millis) && !_holdState[index]) {
        _holdState[index] = true;
        return true;
    }
    return false;
}

//clear hold state when button released
bool LayoutInterface::buttonReleased(Button &button, const byte index) {
    bool released = button.wasReleased();
    if (released && _holdState[index]) {
        _holdState[index] = false;
        return false;
    }
    return released;
}