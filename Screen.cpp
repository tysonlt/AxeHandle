#include "Screen.h"

void Screen::init() { 
  _tft.begin(_tft.readID());
  _tft.setRotation(SCREEN_ROTATION);
  drawBootSplash();
}

void Screen::clearScreen(const Colour colour) {
  _tft.fillScreen(colour);
}

void Screen::setTunerMode(bool enabled) {
  clearScreen();
  _tunerEngaged = enabled;
  if (_tunerEngaged) {
    drawTunerGrid();
  } else {
    _forceNextDisplay = true;
    displayPreset(_lastPreset);
    displayTempo(_lastTempo);
    _forceNextDisplay = false;
  } 

}

void Screen::drawTunerGrid() {
  
  const byte points = (TUNER_RESOLUTION * 2) + 2;
  const byte gap = TUNER_GRID_GAP;
  const byte h = TUNER_GRID_HEIGHT;
  const Colour bright = COLR(TUNER_FINETUNE);
  const Colour dark = DARK_GREEN;

  ScreenPoint p = POS(TUNER_DISPLAY);
  Colour c = bright;
  byte stretch = 0;

  for (byte i=0; i<=points; i++) {
    c = bright;
    stretch = 0;
    if (i%10) {
      c = dark;
    } else if (i == points/2) {
      stretch = 6;
    } else {
      stretch = 1;
    }
    _tft.drawFastVLine(p.x, p.y-stretch, h+(stretch*2), c);
    p.x += gap;
  }
}
    
//Assumes screen has already been cleared    
void Screen::displayTunerData(const char *note, const byte string, const byte fineTune) {
  
  const byte sz = 8;
  char buf[sz];
  int cents = fineTune - TUNER_CENTRE;
  ScreenPoint p = POS(TUNER_DISPLAY);

  printText(note, POS(TUNER_NOTE), FONT(TUNER_NOTE), COLR(TUNER_NOTE));

  snprintf(buf, sz, "%d", string);
  printText(buf, POS(TUNER_STRING), FONT(TUNER_STRING), COLR(TUNER_STRING));
  
  snprintf(buf, sz, "%03d", cents);
  printText(buf, POS(TUNER_FINETUNE), FONT(TUNER_FINETUNE), COLR(TUNER_FINETUNE));

  byte offset = TUNER_GRID_GAP * 3;
  unsigned width = TUNER_RESOLUTION * TUNER_GRID_GAP * 2;
  unsigned x, y = p.y + TUNER_GRID_HEIGHT + 10;
  if (fineTune != _lastFineTune) {
    x = map(_lastFineTune, TUNER_MIN, TUNER_MAX, p.x, width);
    fillTriangle({x+offset, y}, TUNER_ARROW_HEIGHT, COLOUR_BACKGROUND);
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
  fillTriangle({x+offset, y}, TUNER_ARROW_HEIGHT, colour);

  _lastFineTune = fineTune;

}

void Screen::fillTriangle(ScreenPoint p, byte width, Colour colour) {
  _tft.fillTriangle(
    p.x-width/2, p.y+width, 
    p.x, p.y, 
    p.x+width/2, p.y+width, 
    colour
  );
}

void Screen::fillCircle(ScreenPoint p, byte radius, Colour colour) {
  _tft.fillCircle(p.x, p.y, radius, colour);
}

void Screen::drawBootSplash() {
  _booting = true;
  clearScreen();
  printText("Connecting...", {0, 0}, FONT_SIZE_3);
}

void Screen::displayTempo(Tempo tempo) {
	char buf[50];
  if (_tunerEngaged) return;
  checkBooted();
	snprintf(buf, getMaxCharsPerLine() + 1, "TEMPO: %d BPM   ", tempo);
	printText(buf, POS(FOOTER), FONT(FOOTER), COLR(FOOTER));
	drawLine(POS(FOOTER_LINE), COLR(FOOTER_LINE));
  _lastTempo = tempo;
}

void Screen::displayPreset(AxePreset preset) {
  char buf[40];

  if (_tunerEngaged) return;
  checkBooted();
  drawLine(POS(HEADER_LINE), COLR(PRESET_NUMBER));

  if (_forceNextDisplay || isPresetChanged(preset)) {
    snprintf(buf, sizeof(buf), "%03d", preset.getPresetNumber());
    printText(buf, POS(PRESET_NUMBER), FONT(PRESET_NUMBER), COLR(PRESET_NUMBER), OPTION_INVERT);
    preset.copyPresetName(buf, sizeof(buf));
    printText(buf, POS(PRESET_TITLE), FONT(PRESET_TITLE), COLR(PRESET_TITLE),	OPTION_WRAP);
  } 

  if (_forceNextDisplay || isSceneChanged(preset)) {
    snprintf(buf, sizeof(buf), "%d", preset.getSceneNumber());
    printText(buf, POS(SCENE_NUMBER), FONT(SCENE_NUMBER), COLR(SCENE_NUMBER), OPTION_INVERT);
    preset.copySceneName(buf, sizeof(buf));
    printText(buf, POS(SCENE_TITLE), FONT(SCENE_TITLE), COLR(SCENE_TITLE), OPTION_WRAP);
  }

  if (_forceNextDisplay || isPresetChanged(preset) || preset.effectsChanged(_lastPreset)) {
    displayEffects(preset);
  }

  _lastPreset = preset;

}

void Screen::displayEffects(AxePreset preset) {
	
  if (_tunerEngaged) return;

  size_t sz = 5;
	char buf[sz];
	FontSize fontSize = FONT(EFFECTS);

	ScreenPoint p = POS(EFFECTS);
  byte padding = 5;
  byte originalX = p.x;
	byte badgeWidth = (CHAR_W * fontSize * (sz - 1)) + padding; 
	byte effectsPerLine = width() / badgeWidth;
  byte maxEffects = effectsPerLine * 2;
  bool firstLine = true;
	
  //clear previous 
	fillRect(p, {width(), (unsigned) (padding + CHAR_H * fontSize * 2)}, COLOUR_BACKGROUND);

	for (byte i=0, displayCount=0; i < preset.getEffectCount() && displayCount < maxEffects; i++) {
    AxeEffect effect = preset.getEffectAt(i);
    effect.copyEffectTag(buf, sz);
    printText(buf, p, fontSize, COLR(EFFECTS), effect.isBypassed() ? OPTION_BORDER : OPTION_INVERT);
    displayCount++;
    if (firstLine && displayCount >= effectsPerLine) {
      firstLine = false;
      p.x = originalX;
      p.y += fontSize * CHAR_H + padding - 2;
    } else {
      p.x += badgeWidth;
    }
	}
	
}

void Screen::checkBooted() {
  if (_booting) {
    _booting = false;
    clearScreen();
  }
}

bool Screen::isPresetChanged(AxePreset preset) {
  return preset.getPresetNumber() != _lastPreset.getPresetNumber();
}

bool Screen::isSceneChanged(AxePreset preset) {
  return isPresetChanged(preset) || preset.getSceneNumber() != _lastPreset.getSceneNumber();
}

void Screen::displayFirmwareVersion(Version version) {

}

void Screen::printText(const char *text, const ScreenPoint p, 
                const FontSize textSize, const Colour colour, 
                const displayOption_t options) 
{    
  
  byte nudgeX = 0, nudgeY = 0;
  if (options & OPTION_INVERT) {
    nudgeX = 2, nudgeY = 1; 
    _tft.setTextColor(COLOUR_BACKGROUND);  
    _tft.fillRoundRect(p.x, p.y, (CHAR_W * textSize * strlen(text)) + nudgeX, CHAR_H * textSize, 2, colour);
  } else {
    _tft.setTextColor(colour, COLOUR_BACKGROUND);  
  }           

  if (options & OPTION_BORDER) nudgeX += 2;
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

void Screen::drawLine(ScreenPoint p, Colour colour) {
  _tft.drawLine(p.x, p.y, _tft.width(), p.y, colour);
}

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
      return FONT_SIZE_5;
    case FOOTER:
      return FONT_SIZE_2;
    case TUNER_NOTE:
    case TUNER_STRING:
    case TUNER_FINETUNE:
      return FONT_SIZE_7;
    default:
      return FONT_SIZE_3;
  }
}

ScreenPoint Screen::POS(ElementPosition pos) {
  FontSize currentFont = FONT(pos);
  switch (pos) {
    case PRESET_NUMBER: 
    case TUNER_NOTE:
      return { 1, 1 };
    case SCENE_NUMBER:
    case TUNER_STRING:
      return { (unsigned) (width() - 5 - currentFont * CHAR_W), 1 };
    case HEADER_LINE:
      return { 0, (unsigned) (FONT(PRESET_NUMBER) * CHAR_H + 5) };
    case PRESET_TITLE:
      return { 0, (unsigned) (POS(HEADER_LINE).y + 5) };
    case SCENE_TITLE:
      return { 0, (unsigned) (5 + POS(PRESET_TITLE).y + CHAR_H * 2 * FONT(PRESET_TITLE)) };
    case EFFECTS:
      return { 0, (unsigned) (10 + POS(SCENE_TITLE).y + CHAR_H * 2 * FONT(SCENE_TITLE)) };
    case TUNER_FINETUNE:
      return { width() / 2 - currentFont * CHAR_W * 2, height() / 2 - (currentFont * CHAR_W) / 2 };
    case TUNER_DISPLAY:
      return {10, (height() / 3) * 2};
    case FOOTER_LINE:
      return { 0, (unsigned) (POS(FOOTER).y - 5) };
    case FOOTER:
      return { 1, (unsigned) (height() - currentFont * CHAR_H) };
    default:
      return {0, 0};
  }
}

unsigned int Screen::rainbow(byte value) {

  // Value is expected to be in range 0-127
  // The value is converted to a spectrum colour from 0 = blue through to 127 = red

  byte red = 0; // Red is the top 5 bits of a 16 bit colour value
  byte green = 0;// Green is the middle 6 bits
  byte blue = 0; // Blue is the bottom 5 bits

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

