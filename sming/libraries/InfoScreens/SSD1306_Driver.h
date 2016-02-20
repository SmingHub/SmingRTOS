/*
 * SSD1306_Driver.h
 *
 *  Created on: Aug 17, 2015
 *      Author: ilan
 */

#ifndef INCLUDE_SSD1306_DRIVER_H_
#define INCLUDE_SSD1306_DRIVER_H_

#include <Libraries/Adafruit_SSD1306/Adafruit_SSD1306.h>
#include "Base_Display_Driver.h"

class SSD1306_Driver: public Base_Display_Driver, public Adafruit_SSD1306 {
public:
	SSD1306_Driver(int8_t RST);
	void init();
	int16_t getCursorX();
	int16_t getCursorY();
	textRect* print(const String &s);
	void printToLoc(const String &s, textRect &t, int textColor);
	void writeover(textRect &orig, const String &s, bool inverse = false);
	textRect* getCurrentRect();
	int getMaxLines();
	int getBlack();
	int getWhite();

	void clearDisplay(void);
	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	void display();
	void setCursor(int16_t x, int16_t y);
	void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
	void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
	void drawPixel(int16_t x, int16_t y, uint16_t color);
	size_t println(void);
	void setTextSize(uint8_t s);
	void setTextColor(uint16_t c);
};
#endif /* INCLUDE_SSD1306_DRIVER_H_ */
