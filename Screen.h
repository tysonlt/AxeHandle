#pragma once
#include <MCUFRIEND_kbv.h>   
#include <AxeFxControl.h>

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
	FOOTER
};

class Screen {

  public:

		void init();
    void clearScreen();

		void printPreset(AxePreset preset);
		void printTempo(Tempo tempo);
		void printFirmwareVersion(Version version);

  private:

		void printEffects(AxePreset preset);
    void checkBooted();
		ScreenPoint getElementPosition(ElementPosition pos);
    FontSize getElementFontSize(ElementPosition pos);
		void drawLine(ScreenPoint p, Colour colour);
		void fillRect(ScreenPoint from, ScreenPoint to, Colour colour);

    unsigned getMaxCharsPerLine(FontSize fontSize = FONT_SIZE_1) { return MAX_CHARS_PER_LINE / fontSize; }

		void printText(const char *text, const ScreenPoint p = {0, 0}, 
										const FontSize textSize = FONT_SIZE_1, const Colour colour = COLOUR_TITLE, 
										const displayOption_t options = 0);

    void drawBootSplash();

    byte getScreenRotation() { return SCREEN_ROTATION; }
    unsigned width() { return _tft.width(); }
		unsigned height() { return _tft.height(); }

		bool isPresetChanged(AxePreset preset);
		bool isSceneChanged(AxePreset preset);

		const static byte SCREEN_ROTATION 		= 3;
    const static byte MAX_CHARS_PER_LINE 	= 52;

		//Basic colours
    const static Colour BLACK            = 0x0000;
    const static Colour BLUE             = 0x001F;
    const static Colour RED              = 0xF800;
    const static Colour GREEN            = 0x07E0;
    const static Colour CYAN             = 0x07FF;
    const static Colour MAGENTA          = 0xF81F;
    const static Colour YELLOW           = 0xFFE0;
    const static Colour WHITE            = 0xFFFF;
    
    //System colours 
    const static Colour COLOUR_BACKGROUND  = BLACK;
    const static Colour COLOUR_PRESET_NUM  = YELLOW;
    const static Colour COLOUR_TITLE       = YELLOW;
    const static Colour COLOUR_SCENE       = CYAN;
    const static Colour COLOUR_FOOTER      = MAGENTA;
    const static Colour COLOUR_ERROR       = RED;

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

    bool _booting = false;
    MCUFRIEND_kbv _tft;
		AxePreset _lastPreset;

};