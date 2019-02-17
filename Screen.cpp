#include "Screen.h"

void Screen::init() { 
  _tft.begin(_tft.readID());
  _tft.setRotation(SCREEN_ROTATION);
  drawBootSplash();
}

void Screen::clearScreen() {
  _tft.fillScreen(COLOUR_BACKGROUND);
}

void Screen::printTempo(Tempo tempo) {

	char buf[50];

  checkBooted();
	snprintf(buf, getMaxCharsPerLine() + 1, "TEMPO: %d BPM   ", tempo);
	printText(buf, POS(FOOTER), FONT(FOOTER), COLOUR_FOOTER);
	drawLine(POS(FOOTER_LINE), COLOUR_FOOTER);
}

void Screen::printPreset(AxePreset preset) {

  char buf[40];

  checkBooted();
  drawLine(POS(HEADER_LINE), COLOUR_PRESET_NUM);

  if (isPresetChanged(preset)) {
    snprintf(buf, sizeof(buf), "%03d", preset.getPresetNumber());
    printText(buf, POS(PRESET_NUMBER), FONT(PRESET_NUMBER), COLOUR_PRESET_NUM, OPTION_INVERT);
    preset.copyPresetName(buf, sizeof(buf));
    printText(buf, POS(PRESET_TITLE), FONT(PRESET_TITLE), COLOUR_TITLE,	OPTION_WRAP);
  } 

  if (isSceneChanged(preset)) {
    snprintf(buf, sizeof(buf), "%d", preset.getSceneNumber());
    printText(buf, POS(SCENE_NUMBER), FONT(SCENE_NUMBER), COLOUR_SCENE, OPTION_INVERT);
    preset.copySceneName(buf, sizeof(buf));
    printText(buf, POS(SCENE_TITLE), FONT(SCENE_TITLE), COLOUR_SCENE, OPTION_WRAP);
  }

  if (isPresetChanged(preset) || preset.effectsChanged(_lastPreset)) {
    printEffects(preset);
  }

  _lastPreset = preset;

}

void Screen::printEffects(AxePreset preset) {

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
    printText(buf, p, fontSize, COLOUR_EFFECT, effect.isBypassed() ? OPTION_BORDER : OPTION_INVERT);
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

void Screen::printFirmwareVersion(Version version) {

}

void Screen::drawBootSplash() {
  _booting = true;
  clearScreen();
  printText("Connecting...", {0, 0}, FONT_SIZE_3);
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

FontSize Screen::FONT(ElementPosition pos) {
  switch (pos) {
    case PRESET_NUMBER: 
    case SCENE_NUMBER:
      return FONT_SIZE_5;
    case FOOTER:
      return FONT_SIZE_2;
    default:
      return FONT_SIZE_3;
  }
}

ScreenPoint Screen::POS(ElementPosition pos) {
  switch (pos) {
    case PRESET_NUMBER: 
      return { 1, 1 };
    case SCENE_NUMBER:
      return { (unsigned) (width() - 5 - FONT(SCENE_NUMBER) * CHAR_W), 1 };
    case HEADER_LINE:
      return { 0, (unsigned) (FONT(PRESET_NUMBER) * CHAR_H + 5) };
    case PRESET_TITLE:
      return { 0, (unsigned) (POS(HEADER_LINE).y + 5) };
    case SCENE_TITLE:
      return { 0, (unsigned) (5 + POS(PRESET_TITLE).y + CHAR_H * 2 * FONT(PRESET_TITLE)) };
    case EFFECTS:
      return { 0, (unsigned) (10 + POS(SCENE_TITLE).y + CHAR_H * 2 * FONT(SCENE_TITLE)) };
    case FOOTER_LINE:
      return { 0, (unsigned) (POS(FOOTER).y - 5) };
    case FOOTER:
      return { 1, (unsigned) (height() - FONT(FOOTER) * CHAR_H) };
    default:
      return {0, 0};
  }
}