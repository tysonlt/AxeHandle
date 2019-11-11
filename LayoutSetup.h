#pragma once

#include "Hardware.h"
#include "LayoutInterface.h"
#include <SC_Button.h>
#include "LayoutUser.h"

//save custom button assignments to EEPROM
//use with LayoutUser to recall the button actions
//WIP
class LayoutSetup : public LayoutInterface {

  friend class InputManager;

public:
  const char *getName() { return "<SETUP>"; }

protected:

  /////////////////////////////////////////////
  bool read(const byte index, Button &button) {

    if (index == BUTTON_INDEX_TAP && button.pressedFor(1000)) {
      reset();
    }

    if (_button == EMPTY) {
      promptButton(index, button);
      
    } else if (_command == EMPTY) {
      promptCommand(index, button);

    } else if (needsData1()) {
      promptData1(index, button);

    } else if (_command == EFFECT_TOGGLE && _effectBlock == EMPTY) {
      promptEffectBlock(index, button);

    } else if (_buttonMode == EMPTY) {
      promptButtonMode(index, button);

    } else {
      promptSave(index, button);

    }
    
    return true;
    
  }

  void promptButton(const byte index, Button& button) {
    if (button.wasPressed()) {
      if (isButtonWritable(index)) {
        _button = index;
        _leds->dimAll();
        _leds->on(index);
        _waiting = false;
      } else {
        _screen->printText("CAN'T CHANGE MODE OR TUNER", {0, 60}, 1, Screen::RED);
      }
    } else {
      if (!_waiting) {
        printChoiceInstructions();
      }
    }
  }

  void promptCommand(const byte index, Button& button) {
    if (button.wasPressed()) {
      _command = index;
      _waiting = false;
    } else {
      if (!_waiting) {
        prepareOptionList("Choose function:");
        for (byte i=0; i<8; i++) {
          printOption(i, commandToChar(i));
        }
      }
    }
  }

  void promptData1(const byte index, Button& button) {

    if (button.wasPressed()) {

      if (_command == SCENE_CHANGE) {
        _data1 = index + 1;
        _waiting = false;

      } else if (_command == EFFECT_TOGGLE) {
        _effectChoice = index;
        _data1 = effectChoiceToId(_effectChoice);
        _waiting = false;

      } else {

        if (_i < 3) {
          byte b = index + 1;
          if (b == 10) b = 0;
          _num[_i] = b;
          char buf[2];
          snprintf(buf, 2, "%d", _num[_i]);
          _screen->printText(buf, {(unsigned) (_i * Screen::CHAR_W * 2), _screen->height() / 2}, 2);
          _i++;
          if (_i == 3) {
            _data1 = 100 * _num[0] + 10 * _num[1] + _num[2];
            _waiting = false;
          }
        }

      }

    } else {
      if (!_waiting) {
        printData1Instructions();
      }
    }

  }

  void promptEffectBlock(const byte index, Button& button) {

    if (button.wasPressed()) {
      _effectBlock = index + 1;
      _data1 += index;
      _waiting = false;

    } else {

      if (!_waiting) {
        prepareOptionList("Enter effect block num:");
        ledsOn(0, 3);
      }

    }

  }

  void promptButtonMode(const byte index, Button& button) {
    
    if (button.wasPressed()) {
      _buttonMode = static_cast<ButtonMode>(index);
      _waiting = false;

    } else {
      if (!_waiting) {
        prepareOptionList("Choose button mode:");
        for (byte i=0; i<5; i++) {
          printOption(i, buttonModeToChar(i));
        }
      }

    }

  }

  void printEffectOptions() {
    prepareOptionList("Choose function:");
    for (byte i=0; i<8; i++) {
      printOption(i, effectChoiceToChar(i));
    }
  }

  void prepareOptionList(const char *instructions = "") {
    _waiting = true;
    _leds->dimAll(0);
    clearWindow();
    printCurrentData();
    line(instructions);
  }
  
  void printOption(const byte index, const char *name, bool setLed = true) {
    
    const size_t sz = 5 + strlen(name) + 1;
    char buf[sz];

    if (setLed) {
      _leds->dim(index);
    }

    snprintf(buf, sz, "%d: %s", index + 1, name);
    line(buf);

  }

  void promptSave(const byte index, Button& button) {

    if (index == BUTTON_INDEX_MODE_TUNER && button.pressedFor(1000)) {
      if (save()) {
        clearWindow();
        line("SAVED");
      } else {
        line("ERROR SAVING DATA");
      }
      delay(MESSAGE_DELAY);  
      reset();

    } else {

      if (!_waiting) {
        prepareOptionList();
        printCurrentData(true);
        _leds->on(BUTTON_INDEX_MODE_TUNER);
        _leds->on(BUTTON_INDEX_TAP);
        printSavePrompt();
      }

    }

  }

  void printData1Instructions() {

    switch (_command) {
      case SCENE_CHANGE: 
        prepareOptionList("Enter scene number:");
        ledsOn(0, 7);
        break;

      case MIDI_PC:
        prepareOptionList("Enter PC byte:");
        break;

      case MIDI_CC:
        prepareOptionList("Enter CC byte:");
        break;

      case EFFECT_TOGGLE:
        _leds->dimAll();
        printEffectOptions();
        break;

      default: 
        prepareOptionList("Enter 3-digit byte:");
    }

  }

  bool save() {
    _assignments[_button].command = static_cast<Command>(_command);
    _assignments[_button].mode = static_cast<ButtonMode>(_buttonMode);
    _assignments[_button].data1 = _data1;
    _assignments[_button].data2 = _data2;
    return 0 != EEPROM.writeBlock(LayoutUser::EEPROM_ADDRESS, _assignments, LayoutUser::ELEMENT_COUNT);
  }

  void reset() { 
    EEPROM.readBlock(LayoutUser::EEPROM_ADDRESS, _assignments, LayoutUser::ELEMENT_COUNT);
    debug();
    _button = EMPTY;
    _command = EMPTY;
    _data1 = EMPTY;
    _data2 = EMPTY;
    _buttonMode = EMPTY;
    _effectBlock = EMPTY;
    _effectChoice = EMPTY;
    _i = 0;
    _num[0] = 0;
    _num[1] = 0;
    _num[2] = 0;
    _leds->dimAll(); 
    _screen->clearScreen();
    _waiting = false;
  }

  void ledsOn(const byte from, const byte to, const byte brightness = Leds::LED_DIM) {
    for (byte i=from; i<=to; i++) {
      _leds->dim(i, brightness);
    }
  }

  void printCurrentData(const bool fullMode = false) {

    byte x = 0, y = LINE_HEIGHT;
    byte button =  _button + 1;
    const size_t sz = 50;
    char buf[sz];

    if (fullMode) {

      y = LINE_HEIGHT * 3;

      snprintf(buf, sz, "=== BUTTON %d ===", button);
      _screen->printText(buf, {x, y});
      y += LINE_HEIGHT;

      snprintf(buf, sz, "MODE: %s", buttonModeToChar(_buttonMode));
      _screen->printText(buf, {x, y});
      y += LINE_HEIGHT;

      snprintf(buf, sz, "COMMAND: %s", commandToChar(_command));
      _screen->printText(buf, {x, y});
      y += LINE_HEIGHT;

      if (_data1 != EMPTY) {
        if (_effectChoice != EMPTY) {
          snprintf(buf, sz, "%s: %s %d", data1Name(_command), effectChoiceToChar(_effectChoice), _effectBlock);  
        } else {
          snprintf(buf, sz, "%s: %03d", data1Name(_command), _data1);
        }
        _screen->printText(buf, {x, y});
        y += LINE_HEIGHT;
      }

    } else {

      if (_button != EMPTY) {
        snprintf(buf, sz, "BTN %d:", button);
        _screen->printText(buf, {x, y});
        x += strlen(buf) * Screen::CHAR_W;
      }

      if (_command != EMPTY) {
        snprintf(buf, sz, "%s:", commandToChar(_command));
        _screen->printText(buf, {x, y});
        x += strlen(buf) * Screen::CHAR_W;
      }

      if (_data1 != EMPTY) {
        snprintf(buf, sz, "%03d:", _data1);
        _screen->printText(buf, {x, y});
        x += strlen(buf) * Screen::CHAR_W;
      }

      _p.y += LINE_HEIGHT;

    }

  }

  EffectId effectChoiceToId(const byte index) {
    switch (index) {
      case 0: return ID_FUZZ1; 
      case 1: return ID_DELAY1;
      case 2: return ID_CHORUS1;
      case 3: return ID_FLANGER1;
      case 4: return ID_PHASER1;
      case 5: return ID_COMP1; 
      case 6: return ID_PLEX1; 
      case 7: return ID_REVERB1;
      case 8: return ID_PARAEQ1;
      case 9: return ID_WAH1;
      default: return 0;
    }
  }

  const char *effectChoiceToChar(const byte index) {
    switch (index) {
      case 0: return "DRIVE"; 
      case 1: return "DELAY";
      case 2: return "CHORUS";
      case 3: return "FLANGER";
      case 4: return "PHASER";
      case 5: return "COMP"; 
      case 6: return "PLEX"; 
      case 7: return "REVERB";
      case 8: return "PARAEQ";
      case 9: return "WAH";
      default: return "";
    }
  }

  const char *data1Name(byte command) {
    switch (command) {
      case SCENE_CHANGE:
        return "SCENE";
      case EFFECT_TOGGLE:
        return "EFFECT";
      case MIDI_PC:
        return "PC";
      case MIDI_CC:
        return "CC";
      default:
        return "";
    }
  }

  const char *buttonModeToChar(byte mode) {
    switch (mode) {
      case ON_PRESS: 
        return "PRESS";
      case ON_RELEASE:
        return "RELEASE";
      case ON_HOLD_SHORT:
        return "HOLD (SHORT)";
      case ON_HOLD_MEDIUM:
        return "HOLD (MEDIUM)";
      case ON_HOLD_LONG:
        return "HOLD (LONG)";
      default:
        return "";
    }
  }

  const char *commandToChar(byte command) {
    switch(command) {
      case PRESET_DOWN:
        return "PRESET DOWN";
      case PRESET_UP:
        return "PRESET UP";
      case SCENE_DOWN:
        return "SCENE DOWN";
      case SCENE_UP:
        return "SCENE UP";
      case SCENE_CHANGE:
        return "SCENE SET";
      case EFFECT_TOGGLE:
        return "EFFECT";
      case MIDI_PC:
        return "PC";
      case MIDI_CC:
        return "CC";
      default:
        return "";
    }
  }

  using LayoutInterface::LayoutInterface;
  bool readButton(const byte index, Button &button) { return false; }

private:

  bool needsData1() {
    return _data1 == EMPTY && (
      _command == SCENE_CHANGE ||
      _command == EFFECT_TOGGLE ||
      _command == MIDI_PC ||
      _command == MIDI_CC
    );
  }

  bool needsData2() {
    return _data2 == EMPTY && _command == MIDI_CC;
  }

  bool isButtonWritable(const byte index) {
    return index != BUTTON_INDEX_MODE_TUNER && index != BUTTON_INDEX_TAP;
  }

  void printChoiceInstructions() {
    _screen->clearScreen();
    _waiting = true;
    _leds->dimAll();
    _leds->off(BUTTON_INDEX_TAP);
    _leds->off(BUTTON_INDEX_MODE_TUNER);
    _p = {0, 0};
    line("     >>>>> SETUP <<<<<");
    line("");
    line("Press button to assign:");
    printCancelPrompt();
  }

  void printSavePrompt() {
    _screen->printText("Hold MODE to save", {0, _screen->height() - (LINE_HEIGHT * 2)});
  }

  void printCancelPrompt() {
    _screen->printText("Hold TAP to Cancel", {0, _screen->height() - LINE_HEIGHT});
  }

  void clearWindow() {
    byte start = LINE_HEIGHT;
    _screen->fillRect({0, start}, {_screen->width(), _screen->height() - (LINE_HEIGHT * 3)}, Screen::BLACK);
    _p.y = start;
  }

  void line(const char *text) {
    _screen->printText(text, {0, _p.y});
    _p.y += LINE_HEIGHT;
  }

  void debug() {
    const size_t sz = 60;
    char buf[sz];
    PL_("\n--- EEPROM DUMP ---");
    for (byte i=0; i<LayoutUser::ELEMENT_COUNT; i++) {
      if (i == 4) continue;
      memset(buf, 0, sz);
      snprintf(buf, sz, "BUTTON %d [CMD: %03d, DATA1: %03d, DATA2: %03d, MODE: %03d]",
        i, static_cast<int>(_assignments[i].command), _assignments[i].data1, _assignments[i].data2, _assignments[i].mode
      );
      PL_(buf);
    }
    PL_("------\n");
  }

  static const byte EMPTY = 255;
  static const byte LINE_HEIGHT = 9;
  static const unsigned MESSAGE_DELAY = 1300;

  ButtonAssignment _assignments[NUM_BUTTONS];
  byte _buttonMode = EMPTY;
  ScreenPoint _p = {0, 0};
  byte _button = EMPTY;
  byte _command = EMPTY;
  byte _data1 = EMPTY; 
  byte _data2 = EMPTY;
  byte _effectChoice = EMPTY;
  byte _effectBlock = EMPTY;
  byte _i = 0;
  byte _num[3] = {0, 0, 0};
  bool _waiting = false;

};
