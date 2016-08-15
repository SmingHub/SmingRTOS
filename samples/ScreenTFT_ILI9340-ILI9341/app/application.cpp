//#include <user_config.h>
#include <SmingCore.h>
#include <libraries/Adafruit_ILI9341/Adafruit_ILI9341.h>
#include <c_types.h>
//#include <libraries/Adafruit_GFX/Adafruit_GFX.h>


// If you want, you can define WiFi settings globally in Eclipse Environment Variables
#ifndef WIFI_SSID
    #define WIFI_SSID "PleaseEnterSSID" // Put you SSID and Password here
    #define WIFI_PWD "PleaseEnterPass"
#endif

// SPI: if defined use software SPI, else hardware SPI
//#define _USE_SOFTSPI

/*
 * Hardware SPI mode:
 * GND      (GND)       GND
 * VCC      (VCC)       3.3v
 * SCK      (CLK)       GPIO14
 * SDI      (MOSI)      GPIO13
 * RES      (RESET)     GPIO16
 * DC       (DC)        GPIO0
 * CS       (CS)        GPIO2
 * LED      (BL)        Resistor to 3.3V
 * SDO      (MISO)      <nc>  
 */
//#define TFT_SCLK    14
//#define TFT_MOSI    13
//#define TFT_RST     16
//#define TFT_DC      0
//#define TFT_CS      2
#define TFT_SCLK    14
#define TFT_MOSI    13
#define TFT_MISO    12      // not used, just for softSPI contructor
#define TFT_RST     4
#define TFT_DC      5
#define TFT_CS      15


// in this demo, the same ports for HW and SW SPI are used
#ifdef _USE_SOFTSPI
SPISoft sSPI(TFT_MISO,TFT_MOSI,TFT_SCLK,200);
Adafruit_ILI9341 tft = Adafruit_ILI9341(sSPI, TFT_CS, TFT_DC, TFT_RST);
#else
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
#endif

Timer guiTimer = new Timer(true);   // use Queued Timer to not trigger WD on complex screens

int r=0;

int ara=4,yerara=15;
int u1=100;
int u2=320-(u1+ara);
int s1=0;
int s2=(u1+ara);
int p1=50;

int g=28;
int y=90;
int satir=6;

uint32_t startTime;

String lists[]={"a","b","c","d","e","f"};



void testdrawtext(const char text[], uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void tftPrintTest1() {
  tft.setTextWrap(false);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(1);
  tft.println("Hello Sming!");
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.println("Hello Sming!");
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(3);
  tft.println("Hello Sming!");
  tft.setTextColor(ILI9341_BLUE);
  tft.setTextSize(4);
  tft.print(1234.567);
}

void testlines(uint16_t color) {
    tft.fillScreen(ILI9341_BLACK);
    for (int16_t x = 0; x < tft.width(); x += 6) {
        tft.drawLine(0, 0, x, tft.height() - 1, color);
    }
    for (int16_t y = 0; y < tft.height(); y += 6) {
        tft.drawLine(0, 0, tft.width() - 1, y, color);
    }

    tft.fillScreen(ILI9341_BLACK);
    for (int16_t x = 0; x < tft.width(); x += 6) {
        tft.drawLine(tft.width() - 1, 0, x, tft.height() - 1, color);
    }
    for (int16_t y = 0; y < tft.height(); y += 6) {
        tft.drawLine(tft.width() - 1, 0, 0, y, color);
    }

    tft.fillScreen(ILI9341_BLACK);
    for (int16_t x = 0; x < tft.width(); x += 6) {
        tft.drawLine(0, tft.height() - 1, x, 0, color);
    }
    for (int16_t y = 0; y < tft.height(); y += 6) {
        tft.drawLine(0, tft.height() - 1, tft.width() - 1, y, color);
    }

    tft.fillScreen(ILI9341_BLACK);
    for (int16_t x = 0; x < tft.width(); x += 6) {
        tft.drawLine(tft.width() - 1, tft.height() - 1, x, 0, color);
    }
    for (int16_t y = 0; y < tft.height(); y += 6) {
        tft.drawLine(tft.width() - 1, tft.height() - 1, 0, y, color);
    }
}

void testfastlines(uint16_t color1, uint16_t color2) {
    tft.fillScreen(ILI9341_BLACK);
    for (int16_t y = 0; y < tft.height(); y += 5) {
        tft.drawFastHLine(0, y, tft.width(), color1);
    }
    for (int16_t x = 0; x < tft.width(); x += 5) {
        tft.drawFastVLine(x, 0, tft.height(), color2);
    }
}

void testdrawrects(uint16_t color) {
    tft.fillScreen(ILI9341_BLACK);
    for (int16_t x = 0; x < tft.width(); x += 6) {
        tft.drawRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2, x, x, color);
    }
}

void testfillrects(uint16_t color1, uint16_t color2) {
    tft.fillScreen(ILI9341_BLACK);
    for (int16_t x = tft.width() - 1; x > 6; x -= 6) {
        tft.fillRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2, x, x, color1);
        tft.drawRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2, x, x, color2);
    }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
    for (int16_t x = 0; x < tft.width() + radius; x += radius * 2) {
        for (int16_t y = 0; y < tft.height() + radius; y += radius * 2) {
            tft.drawCircle(x, y, radius, color);
        }
    }
}

void testfillcircles(uint8_t radius, uint16_t color) {
    for (int16_t x = radius; x < tft.width(); x += radius * 2) {
        for (int16_t y = radius; y < tft.height(); y += radius * 2) {
            tft.fillCircle(x, y, radius, color);
        }
    }
}

void testtriangles() {
    tft.fillScreen(ILI9341_BLACK);
    int color = 0xF800;
    int t;
    int w = tft.width() / 2;
    int x = tft.height() - 1;
    int y = 0;
    int z = tft.width();
    for (t = 0; t <= 15; t += 1) {
        tft.drawTriangle(w, y, y, x, z, x, color);
        x -= 4;
        y += 4;
        z -= 4;
        color += 100;
    }
}


void basicGui() {
    debugf("running basicGui()");
    startTime = millis();
    //    tft.setTextSize(1);
    tft.setRotation(1);
    tft.setTextSize(3);
    tft.fillRect(s1, 0, u1 * 2, 48, ILI9341_OLIVE);
    tft.setCursor(15, 15);
    tft.println("Sming");
    tft.setTextSize(2);
    tft.fillRect((u1 * 2) + ara, 0, 318 - (u1 * 2), 48, ILI9341_RED);
    for (int a = 0; a < satir; a++) {
        tft.setTextColor(ILI9341_GREEN);
        tft.fillRect(s1, p1, u1, g, ILI9341_DARKCYAN);
        tft.setCursor(s1 + yerara, p1 + 6);
        tft.setTextColor(ILI9341_WHITE);
        tft.println(lists[a]);
        tft.fillRect(s2, p1, u2, g, ILI9341_DARKCYAN);
        tft.setCursor(s2 + yerara, p1 + 6);
        tft.println(r);
        p1 += g + 4;
    }
    p1 = 50;
    r++;
    debugf("GUI displayed in %d ms\n", millis() - startTime);
}

void screen8() {
    debugf("-starting screen8: testtriangles");
    startTime = millis();
    testtriangles();
    debugf("-screen8: testtriangles %d ms", millis() - startTime);
    guiTimer.initializeMs(1000, basicGui).start();
}

void screen7() {
    debugf("-starting screen7: testfillcircles");
    startTime = millis();
    tft.fillScreen(ILI9341_BLACK);
    testfillcircles(10, ILI9341_BLUE);
    testdrawcircles(10, ILI9341_WHITE);
    debugf("-screen7: testfillcircles %d ms", millis() - startTime);
    guiTimer.initializeMs(1000, screen8).start(FALSE);
}

void screen6() {
    debugf("-starting screen6: testfillrects");
    startTime = millis();
    testfillrects(ILI9341_YELLOW, ILI9341_MAGENTA);
    debugf("-screen6: testfillrects %d ms", millis() - startTime);
    guiTimer.initializeMs(1000, screen7).start(FALSE);
}

void screen5() {
    debugf("-starting screen5: testdrawrects");
    startTime = millis();
    testdrawrects(ILI9341_GREEN);
    debugf("-screen5: testdrawrects %d ms", millis() - startTime);
    guiTimer.initializeMs(1000, screen6).start(FALSE);
}

void screen4() {
    debugf("-starting screen4: testfastlines");
    startTime = millis();
    // optimized lines
    testfastlines(ILI9341_RED, ILI9341_BLUE);
    debugf("-screen4: testfastlines %d ms", millis() - startTime);
    guiTimer.initializeMs(1000, screen5).start(FALSE);
}

void screen3() {
    debugf("-starting screen3: testlines");
    startTime = millis();
    // line draw test
    testlines(ILI9341_YELLOW);
    debugf("-screen3: testlines %d ms", millis() - startTime);
    guiTimer.initializeMs(1000, screen4).start(FALSE);
}

void screen2() {
    debugf("-starting screen2: tftPrintTest");
    startTime = millis();
    tftPrintTest1();
    debugf("-screen2: tftPrintTest %d ms", millis() - startTime);
    guiTimer.initializeMs(1000, screen3).start(FALSE);
}

void screen1() {
    debugf("-starting screen1: testdrawtext");
    startTime = millis();
    // large block of text
    tft.fillScreen(ILI9341_BLACK);
    testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", ILI9341_WHITE);
    debugf("-screen1: testdrawtext %d ms", millis() - startTime);
    guiTimer.initializeMs(1000, screen2).start(FALSE);
}



void init() {
    Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
    Serial.systemDebugOutput(true); // Allow debug output to serial
    Serial.commandProcessing(false);
    //WifiStation.config(WIFI_SSID, WIFI_PWD);
    WifiStation.enable(false);
    //WifiAccessPoint.enable(false);
    //  delay(2000);
    debugf("Display start");
    startTime = millis();
    
    // text display tests
    //tft.begin();
    tft.init();
    //tft.init(4000000);        // optionally init with specific SPI speed (HWSPI)
    debugf("Init done");
    debugf("-clearscreen\n");
    tft.fillScreen(ILI9341_BLACK);
    debugf("-Initialized in %d ms\n", millis() - startTime);
    
    tft.setRotation(1);
    debugf("-setrotation(1) done");
    tft.setTextSize(2);
    debugf("-seTextSize(2) done");
    tft.setTextColor(ILI9341_GREEN);
    tft.setCursor(0, 0);
    tft.setCursor(60, 60);
    tft.println("Sming  Framework");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); // text
    tft.setCursor(60, 75);
    tft.println("              v1.1_rtos");
    tft.setTextColor(ILI9341_CYAN);
    tft.setCursor(60, 90);
    tft.println("ili9340-40C-41 ");
    tft.setCursor(60, 125);
    tft.println("have fun with Sming");
    debugf("initial print done");
    //delay(2000);
    //debugf("delay done");
    tft.fillScreen(ILI9341_BLACK);
    debugf("fillScreen(0) done");
    guiTimer.initializeMs(1000, screen1).start(FALSE);
    debugf("timer task started");
}
