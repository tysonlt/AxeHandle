#pragma once
#include <MCUFRIEND_kbv.h>   
#include <AxeFxControl.h>
#include "InputManager.h"
#include "Hardware.h"

typedef byte FontSize;
typedef byte displayOption_t;
typedef uint16_t Colour;

struct ScreenPoint {
  unsigned x;
  unsigned y;
};

enum ElementPosition {
	PRESET_NUMBER,
	SCENE_NUMBER,
	HEADER_LINE,
	PRESET_TITLE,
	SCENE_TITLE,
	EFFECTS,
	FOOTER_LINE,
	FOOTER,
  TEMPO,
  TUNER_NOTE,
  TUNER_STRING,
  TUNER_FINETUNE,
  TUNER_DISPLAY,
  LAYOUT
};

class Screen {

  public:

		void init();
    void clearScreen(const Colour colour = COLOUR_BACKGROUND);

		void displayPreset(AxePreset preset);
		void displayTempo(Tempo tempo);
		void displayFirmwareVersion(Version version);
    void displayLayout(LayoutType layout);
    void setTunerMode(bool enabled);
    void displayTunerData(const char *note, const byte string, const byte fineTune);

  private:

		const static byte SCREEN_ROTATION 		= 3;
    const static byte MAX_CHARS_PER_LINE 	= 52;
    const static byte TUNER_CENTRE 	      = 63;
    const static byte TUNER_RESOLUTION    = 49; //either side of 63
    constexpr static int8_t TUNER_MIN     = TUNER_CENTRE - TUNER_RESOLUTION;
    constexpr static int8_t TUNER_MAX     = TUNER_CENTRE + TUNER_RESOLUTION;
    const static byte TUNER_ARROW_HEIGHT  = 25;
    const static byte TUNER_GRID_HEIGHT   = 30;
    const static byte TUNER_GRID_GAP      = 3;

		//Basic colours
    const static Colour BLACK            = 0x0000;
    const static Colour BLUE             = 0x001F;
    const static Colour RED              = 0xF800;
    const static Colour GREEN            = 0x07E0;
    const static Colour DARK_GREEN       = 0x03E0;
    const static Colour CYAN             = 0x07FF;
    const static Colour MAGENTA          = 0xF81F;
    const static Colour YELLOW           = 0xFFE0;
    const static Colour WHITE            = 0xFFFF;
    const static Colour GREY             = 0x2104;

    const static Colour COLOUR_BACKGROUND  = BLACK;
    const static Colour COLOUR_ERROR       = RED;
    const static Colour COLOUR_DEFAULT     = YELLOW;
    const static Colour COLOUR_TUNER_OK    = WHITE;
    
    //dimensions of printable areas
    const static byte SCREEN_OFFSET          = 1;
    const static byte CHAR_W                 = 6;
    const static byte CHAR_H                 = 8;

		//Font
    const static FontSize FONT_SIZE_1        = 1;
    const static FontSize FONT_SIZE_2        = 2;
    const static FontSize FONT_SIZE_3        = 3;
    const static FontSize FONT_SIZE_4        = 4;
    const static FontSize FONT_SIZE_5        = 5;
    const static FontSize FONT_SIZE_6        = 6;
    const static FontSize FONT_SIZE_7        = 7;

    //print options
		const static displayOption_t OPTION_INVERT 	= 1<<0;
		const static displayOption_t OPTION_WRAP		= 1<<1;
		const static displayOption_t OPTION_BORDER	= 1<<2;

		void displayEffects(AxePreset preset);
    void checkBooted();
		void drawLine(ScreenPoint p, Colour colour);
		void fillRect(ScreenPoint from, ScreenPoint to, Colour colour);
    void fillCircle(ScreenPoint p, byte radius, Colour colour);
    void fillTriangle(ScreenPoint p, byte width, Colour colour);

    ScreenPoint POS(ElementPosition pos);
    FontSize FONT(ElementPosition pos);
    Colour COLR(ElementPosition pos);

    unsigned getMaxCharsPerLine(FontSize fontSize = FONT_SIZE_1) { return MAX_CHARS_PER_LINE / fontSize; }

		void printText(const char *text, const ScreenPoint p = {0, 0}, 
										const FontSize textSize = FONT_SIZE_1, const Colour colour = COLOUR_DEFAULT, 
										const displayOption_t options = 0);

    void printText(const char *text, ElementPosition pos, const displayOption_t options = 0);

    void drawBootSplash();
    void drawTunerGrid();

    byte getScreenRotation() { return SCREEN_ROTATION; }
    unsigned width() { return _tft.width(); }
		unsigned height() { return _tft.height(); }

    unsigned int rainbow(byte value);

    bool _booting = false;
    bool _tunerEngaged = false;
    byte _lastFineTune = 0;
		AxePreset _lastPreset;
    Tempo _lastTempo;
    LayoutType _lastLayout;
    MCUFRIEND_kbv _tft;

};