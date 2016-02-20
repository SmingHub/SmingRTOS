/*
 * SSD1306_Driver.cpp
 *
 *  Created on: Aug 17, 2015
 *      Author: ilan
 */
#include "SSD1306_Driver.h"

// initializer for I2C - we only indicate the reset pin!
SSD1306_Driver::SSD1306_Driver(int8_t RST) : Base_Display_Driver(), Adafruit_SSD1306(RST)
{

}

void SSD1306_Driver::init() {
	Adafruit_SSD1306::begin(SSD1306_SWITCHCAPVCC);
	setTextColor(WHITE);
	setTextSize(1);
	display();
}

int16_t SSD1306_Driver::getCursorX() {
	return cursor_x;
}

int16_t SSD1306_Driver::getCursorY() {
	return cursor_y;
}

textRect* SSD1306_Driver::print(const String &s){
	textRect *t = new textRect();
	t->x = cursor_x;
	t->y = cursor_y;
	t->h = 8;
	Adafruit_SSD1306::print(s);
	t->w = cursor_x - t->x;
	return t;
}

void SSD1306_Driver::printToLoc(const String &s, textRect &t, int color = WHITE){
	setCursor(t.x, t.y);
	int tempTextColor = textcolor;
	setTextColor(color);

	textRect* newt = this->print(s);
	t.x = newt->x;
	t.y = newt->y;
	t.h = newt->h;
	t.w = newt->w;

	delete(newt);

	setTextColor(tempTextColor);
}

void SSD1306_Driver::writeover(textRect &orig, const String &s, bool inverse) {
	int textColor = WHITE;
	int bkColor = BLACK;
	if (inverse) {
		textColor = BLACK;
		bkColor = WHITE;
	}

	this->fillRect( orig.x, orig.y, orig.w, orig.h, bkColor);
	this->printToLoc(s, orig, textColor);
}

textRect* SSD1306_Driver::getCurrentRect() {
	textRect *t = new textRect();
	t->x = cursor_x;
	t->y = cursor_y;

	return t;
}

int SSD1306_Driver::getMaxLines() {
	return 6;
}

int SSD1306_Driver::getBlack() {
	return BLACK;
}

int SSD1306_Driver::getWhite() {
	return WHITE;
}

void SSD1306_Driver::clearDisplay(void){
	Adafruit_SSD1306::clearDisplay();
}

void SSD1306_Driver::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
	Adafruit_SSD1306::fillRect(x, y, w, h, color);
}

void SSD1306_Driver::display(){
	Adafruit_SSD1306::display();
}

void SSD1306_Driver::setCursor(int16_t x, int16_t y){
	Adafruit_SSD1306::setCursor(x, y);
}

void SSD1306_Driver::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color){
	Adafruit_SSD1306::drawFastVLine(x, y, h, color);
}

void SSD1306_Driver::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color){
	Adafruit_SSD1306::drawFastHLine(x, y, w, color);
}

void SSD1306_Driver::drawPixel(int16_t x, int16_t y, uint16_t color){
	Adafruit_SSD1306::drawPixel(x, y, color);
}

size_t SSD1306_Driver::println(void){
	return Adafruit_SSD1306::println();
}

void SSD1306_Driver::setTextSize(uint8_t s){
	Adafruit_SSD1306::setTextSize(s);
}

void SSD1306_Driver::setTextColor(uint16_t c){
	Adafruit_SSD1306::setTextColor(c);
}
