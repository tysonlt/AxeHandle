#include "Screen.h"

void Screen::init() {
  _tft.initR(INITR_BLACKTAB);
  _tft.setRotation(SCREEN_ROTATION);
  drawBootSplash();
}

void Screen::clearScreen(const Colour colour) { _tft.fillScreen(colour); }

void Screen::setTunerMode(bool enabled) {
  clearScreen();
  _tunerEngaged = enabled;
  if (_tunerEngaged) {
    drawTunerGrid();
  } else {
    _forceNextDisplay = true;
    displayPreset(_lastPreset);
    displayTempo(_lastTempo);
    displayLayout(_lastLayout);
    _forceNextDisplay = false;
  }
}

void Screen::drawTunerGrid() {}

// Assumes screen has already been cleared
void Screen::displayTunerData(const char *note, const byte string, const byte fineTune) {

  const byte sz = 8;
  char buf[sz];
  int cents = fineTune - TUNER_CENTRE;
  ScreenPoint p = POS(TUNER_DISPLAY);

  printText(note, TUNER_NOTE);

  snprintf(buf, sz, "%d", string);
  printText(buf, TUNER_STRING);

  snprintf(buf, sz, "%03d", cents);
  printText(buf, TUNER_FINETUNE);

  byte offset = TUNER_GRID_GAP * 3;
  unsigned width = _tft.width() - 10;
  unsigned x, y = p.y;
  if (fineTune != _lastFineTune) {
    x = map(_lastFineTune, TUNER_MIN, TUNER_MAX, p.x, width);
    fillTriangle({x + offset, y}, TUNER_ARROW_HEIGHT, COLOUR_BACKGROUND);
  }

  unsigned int colour;
  if (cents == 0) {
    colour = COLOUR_TUNER_OK;
  } else if (cents < 0) {
    colour = rainbow(map(cents, -TUNER_RESOLUTION, 0, 127, 63));
  } else {
    colour = rainbow(map(cents, 0, TUNER_RESOLUTION, 63, 127));
  }
  x = map(fineTune, TUNER_MIN, TUNER_MAX, p.x, width);
  fillTriangle({x + offset, y}, TUNER_ARROW_HEIGHT, colour);

  _lastFineTune = fineTune;
}

void Screen::fillTriangle(ScreenPoint p, byte width, Colour colour) {
  _tft.fillTriangle(p.x - width / 2, p.y + width, p.x, p.y, p.x + width / 2, p.y + width, colour);
}

void Screen::fillCircle(ScreenPoint p, byte radius, Colour colour) { _tft.fillCircle(p.x, p.y, radius, colour); }

void Screen::drawBootSplash() {
  _booting = true;
  clearScreen();
  printText("Connecting...", SPLASH);
}

void Screen::displayTempo(Tempo tempo) {
  char buf[50];
  if (_tunerEngaged)
    return;
  checkBooted();
  snprintf(buf, getMaxCharsPerLine() + 1, "%d BPM   ", tempo);
  printText(buf, TEMPO);
  _lastTempo = tempo;
}

void Screen::displayPreset(AxePreset preset) {

  const size_t szScene = MAX_CHARS_PER_LINE / FONT(PRESET_TITLE) * 2;
  const size_t szPreset = szScene;
  char buf[szPreset + 1];

  if (_tunerEngaged)
    return;
  checkBooted();

  drawLine(POS(HEADER_LINE), COLR(PRESET_NUMBER));
  drawLine(POS(FOOTER_LINE), COLR(FOOTER_LINE));

  if (_forceNextDisplay || !preset.equals(_lastPreset)) {

    snprintf(buf, 4, "%03d", preset.getPresetNumber());
    printText(buf, PRESET_NUMBER, OPTION_INVERT);
    preset.copyPresetName(buf, szPreset);
    printText(buf, PRESET_TITLE, OPTION_WRAP);

    snprintf(buf, 2, "%d", preset.getSceneNumber());
    printText(buf, SCENE_NUMBER, OPTION_INVERT);
    preset.copySceneName(buf, szScene);
    printText(buf, SCENE_TITLE, OPTION_WRAP);
  }

  _lastPreset = preset;
}

void Screen::displayLayout(LayoutInterface *layout) {
  if (_booting || _tunerEngaged)
    return;

  const size_t sz = 13;
  char buf[sz];
  snprintf(buf, sz, "%11s", layout->getName());
  printText(buf, LAYOUT);

  _lastLayout = layout;
}

void Screen::checkBooted() {
  if (_booting) {
    _booting = false;
    clearScreen();
  }
}

void Screen::displayFirmwareVersion(Version version) {}

void Screen::printText(const char *text, ElementPosition pos, const displayOption_t options) {
  printText(text, POS(pos), FONT(pos), COLR(pos), options);
}

void Screen::printText(const char *text, const ScreenPoint p, const FontSize textSize, const Colour colour,
                       const displayOption_t options) {

  byte nudgeX = 0, nudgeY = 0;
  if (options & OPTION_INVERT) {
    nudgeX = 2, nudgeY = 1;
    _tft.setTextColor(COLOUR_BACKGROUND);
    _tft.fillRoundRect(p.x, p.y, (CHAR_W * textSize * strlen(text)) + nudgeX, CHAR_H * textSize, 2, colour);
  } else {
    _tft.setTextColor(colour, COLOUR_BACKGROUND);
  }

  if (options & OPTION_BORDER)
    nudgeX += 2;
  _tft.setCursor(p.x + nudgeX, p.y + nudgeY);
  _tft.setTextSize(textSize);
  _tft.setTextWrap(options & OPTION_WRAP);
  _tft.print(text);

  if (options & OPTION_BORDER) {
    _tft.drawRoundRect(p.x, p.y, (CHAR_W * textSize * strlen(text)) + nudgeX, CHAR_H * textSize, 2, colour);
  }
}

void Screen::fillRect(ScreenPoint from, ScreenPoint to, Colour colour) {
  _tft.fillRect(from.x, from.y, to.x, to.y, colour);
}

void Screen::drawLine(ScreenPoint p, Colour colour) { _tft.drawLine(p.x, p.y, _tft.width(), p.y, colour); }

Colour Screen::COLR(ElementPosition pos) {

  switch (pos) {
  case PRESET_NUMBER:
  case PRESET_TITLE:
  case HEADER_LINE:
    return YELLOW;

  case SCENE_NUMBER:
  case SCENE_TITLE:
    return CYAN;

  case EFFECTS:
    return RED;

  case FOOTER_LINE:
  case FOOTER:
  case TEMPO:
  case LAYOUT:
    return MAGENTA;

  case TUNER_NOTE:
  case TUNER_STRING:
  case TUNER_FINETUNE:
    return GREEN;

  default:
    return WHITE;
  }
}

FontSize Screen::FONT(ElementPosition pos) {
  switch (pos) {
  case PRESET_NUMBER:
  case SCENE_NUMBER:
    return FONT_SIZE_3;
  case FOOTER:
  case TEMPO:
  case LAYOUT:
    return FONT_SIZE_1;
  case TUNER_NOTE:
  case TUNER_STRING:
  case TUNER_FINETUNE:
    return FONT_SIZE_3;
  default:
    return FONT_SIZE_2;
  }
}

ScreenPoint Screen::POS(ElementPosition pos) {
  FontSize currentFont = FONT(pos);
  switch (pos) {
  case PRESET_NUMBER:
  case TUNER_NOTE:
    return {1, 1};
  case SCENE_NUMBER:
  case TUNER_STRING:
    return {(unsigned)(width() - 5 - currentFont * CHAR_W), 1};
  case HEADER_LINE:
    return {0, (unsigned)(FONT(PRESET_NUMBER) * CHAR_H + 5)};
  case PRESET_TITLE:
    return {0, (unsigned)(POS(HEADER_LINE).y + 5)};
  case SCENE_TITLE:
    return {0, (unsigned)(5 + POS(PRESET_TITLE).y + CHAR_H * 2 * FONT(PRESET_TITLE))};
  case EFFECTS:
    return {0, (unsigned)(10 + POS(SCENE_TITLE).y + CHAR_H * 2 * FONT(SCENE_TITLE))};
  case TUNER_FINETUNE:
    return {width() / 2 - currentFont * CHAR_W * 2, height() / 2 - (currentFont * CHAR_W) / 2};
  case TUNER_DISPLAY:
    return {10, ((height() / 3) * 2)};
  case FOOTER_LINE:
    return {0, (unsigned)(POS(FOOTER).y - 5)};
  case TEMPO:
  case FOOTER:
    return {1, (unsigned)(height() - currentFont * CHAR_H)};
  case LAYOUT:
    return {width() - currentFont * CHAR_W * 12, POS(FOOTER).y};
  default:
    return {0, 0};
  }
}

unsigned int Screen::rainbow(byte value) {

  // Value is expected to be in range 0-127
  // The value is converted to a spectrum colour from 0 = blue through to
  // 127 = red

  byte red = 0;   // Red is the top 5 bits of a 16 bit colour value
  byte green = 0; // Green is the middle 6 bits
  byte blue = 0;  // Blue is the bottom 5 bits

  byte quadrant = value / 32;

  if (quadrant == 0) {
    blue = 31;
    green = 2 * (value % 32);
    red = 0;
  }
  if (quadrant == 1) {
    blue = 31 - (value % 32);
    green = 63;
    red = 0;
  }
  if (quadrant == 2) {
    blue = 0;
    green = 63;
    red = value % 32;
  }
  if (quadrant == 3) {
    blue = 0;
    green = 63 - 2 * (value % 32);
    red = 31;
  }
  return (red << 11) + (green << 5) + blue;
}
