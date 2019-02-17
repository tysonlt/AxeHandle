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
	snprintf(buf, getMaxCharsPerLine() + 1, "TEMPO: %d BPM   ", tempo);
	printText(buf, getElementPosition(FOOTER), getElementFontSize(FOOTER), COLOUR_FOOTER);
	drawLine(getElementPosition(FOOTER_LINE), COLOUR_FOOTER);
}

void Screen::printPreset(AxePreset preset) {

  char buf[40];

  drawLine(getElementPosition(HEADER_LINE), COLOUR_PRESET_NUM);

  if (isPresetChanged(preset)) {
    snprintf(buf, sizeof(buf), "%03d", preset.getPresetNumber());
    printText(buf, getElementPosition(PRESET_NUMBER), getElementFontSize(PRESET_NUMBER), COLOUR_PRESET_NUM, OPTION_INVERT);
    preset.copyPresetName(buf, sizeof(buf));
    printText(buf, getElementPosition(PRESET_TITLE), getElementFontSize(PRESET_TITLE), COLOUR_TITLE,	OPTION_WRAP);
  } 

  if (isSceneChanged(preset)) {
    snprintf(buf, sizeof(buf), "%d", preset.getSceneNumber());
    printText(buf, getElementPosition(SCENE_NUMBER), getElementFontSize(SCENE_NUMBER), COLOUR_SCENE, OPTION_INVERT);
    preset.copySceneName(buf, sizeof(buf));
    printText(buf, getElementPosition(SCENE_TITLE), getElementFontSize(SCENE_TITLE), COLOUR_SCENE, OPTION_WRAP);
  }

  if (isPresetChanged(preset) || preset.effectsChanged(_lastPreset)) {
    printEffects(preset);
  }

  _lastPreset = preset;

}

bool Screen::isPresetChanged(AxePreset preset) {
  return preset.getPresetNumber() != _lastPreset.getPresetNumber();
}

bool Screen::isSceneChanged(AxePreset preset) {
  return isPresetChanged(preset) || preset.getSceneNumber() != _lastPreset.getSceneNumber();
}

void Screen::printFirmwareVersion(Version version) {

}

void Screen::printEffects(AxePreset preset) {

	size_t sz = 5;
	char buf[sz];
	FontSize fontSize = getElementFontSize(EFFECTS);

	ScreenPoint p = getElementPosition(EFFECTS);
  byte originalX = p.x;
	byte badgeWidth = (CHAR_W * fontSize * 4) + 1 + 4; //width of badge with 2-pixel padding 
	byte effectsPerLine = width() / badgeWidth;
  byte maxEffects = effectsPerLine * 2;
  bool firstLine = true;
	
  //clear previous 
	fillRect(p, {width(), (unsigned) (5 + CHAR_H * fontSize * 2)}, COLOUR_BACKGROUND);

	//print all the important effects we can fit (4 on this screen)
	for (byte i=0, displayCount=0; i < preset.getEffectCount() && displayCount < maxEffects; i++) {
    AxeEffect effect = preset.getEffectAt(i);
    effect.copyEffectTag(buf, sz);
    printText(buf, p, fontSize, BLUE, effect.isBypassed() ? OPTION_BORDER : OPTION_INVERT);
    displayCount++;
    if (firstLine && displayCount >= effectsPerLine) {
      firstLine = false;
      p.x = originalX;
      p.y += fontSize * CHAR_H + 3;
    } else {
      p.x += badgeWidth;
    }
	}
	
}

void Screen::drawBootSplash() {
  clearScreen();
}

void Screen::printText(const char *text, const ScreenPoint p, 
                const FontSize textSize, const Colour colour, 
                const displayOption_t options) 
{    
  
  //offset the text if in inverted mode
  byte nudgeX = 0, nudgeY = 0;
  if (options & OPTION_INVERT) {
    
    //we need to draw a rect because the char is drawn too close to the edge
    nudgeX = 2, nudgeY = 1; //push the text further into the box, rather than the hard edge

    //fill rect to inverted back colour
    _tft.setTextColor(COLOUR_BACKGROUND);  
    _tft.fillRoundRect(p.x, p.y, (CHAR_W * textSize * strlen(text)) + nudgeX, CHAR_H * textSize, 2, colour);

  } else {

    //regular draw with flicker-free transparent background
    _tft.setTextColor(colour, COLOUR_BACKGROUND);  
    
  }           

  if (options & OPTION_BORDER) {
    nudgeX += 2;
  }

  //set coords and text behaviour
  _tft.setCursor(p.x + nudgeX, p.y + nudgeY);
  _tft.setTextSize(textSize);
  _tft.setTextWrap(options & OPTION_WRAP);

  //punch it!
  _tft.print(text);

  //draw border rect around text to overwrite hanging background space
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

FontSize Screen::getElementFontSize(ElementPosition pos) {
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

ScreenPoint Screen::getElementPosition(ElementPosition pos) {
  switch (pos) {
    case PRESET_NUMBER: 
      return { 1, 1 };
    case SCENE_NUMBER:
      return { (unsigned) (width() - 5 - getElementFontSize(SCENE_NUMBER) * CHAR_W), 1 };
    case HEADER_LINE:
      return { 0, (unsigned) (getElementFontSize(PRESET_NUMBER) * CHAR_H + 5) };
    case PRESET_TITLE:
      return { 0, (unsigned) (getElementPosition(HEADER_LINE).y + 5) };
    case SCENE_TITLE:
      return { 0, (unsigned) (5 + getElementPosition(PRESET_TITLE).y + CHAR_H * 2 * getElementFontSize(PRESET_TITLE)) };
    case EFFECTS:
      return { 0, (unsigned) (10 + getElementPosition(SCENE_TITLE).y + CHAR_H * 2 * getElementFontSize(SCENE_TITLE)) };
    case FOOTER_LINE:
      return { 0, (unsigned) (getElementPosition(FOOTER).y - 5) };
    case FOOTER:
      return { 1, (unsigned) (height() - getElementFontSize(FOOTER) * CHAR_H) };
    default:
      return {0, 0};
  }
}