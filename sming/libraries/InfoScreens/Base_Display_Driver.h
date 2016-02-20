/*
 * Base_Screen_Driver.h
 *
 *  Created on: Dec 6, 2015
 *      Author: iklein
 */

#ifndef INCLUDE_DRIVERS_BASE_DISPLAY_DRIVER_H_
#define INCLUDE_DRIVERS_BASE_DISPLAY_DRIVER_H_

#include "../../include/SmingCore.h"

struct textRect {
	int x = -1;
	int y =-1;
	int h = -1;
	int w=-1;
};
//

class Base_Display_Driver {
public:
	 Base_Display_Driver(){};
//	virtual ~Base_Display_Driver(){};
	virtual void init()=0;
	virtual int16_t getCursorX() =0;
	virtual int16_t getCursorY()  =0;
	virtual textRect* print(const String &s) =0;
	virtual void printToLoc(const String &s, textRect &t, int textColor) =0;
	virtual void writeover(textRect &orig, const String &s, bool inverse = false) =0;
	virtual textRect* getCurrentRect()  =0;
	virtual int getMaxLines() =0;
	virtual int getBlack() =0;
	virtual int getWhite() =0;

	virtual void clearDisplay(void) =0;
	virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)=0;
	virtual void display() =0;
	virtual void setCursor(int16_t x, int16_t y) =0;
	virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) =0;
	virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) =0;
	virtual void drawPixel(int16_t x, int16_t y, uint16_t color) =0 ;
	virtual size_t println(void) =0;
	virtual void setTextSize(uint8_t s)=0;
	virtual void setTextColor(uint16_t c)=0;
};

#endif /* INCLUDE_DRIVERS_BASE_DISPLAY_DRIVER_H_ */
