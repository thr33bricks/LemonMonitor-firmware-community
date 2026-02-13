/*
 * Project: Lemon Monitor Firmware
 * Author:  Yordan Yordanov
 * Date:    2025-06
 * Github:  https://github.com/thr33bricks/LemonMonitor-firmware-community
 *
 * License: GNU GPL V3 - See LICENSE file for details.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Using a modified version of Adafruit_SSD1306 to
// use a shared buffer for both displays based
// on adafruit/Adafruit SSD1306@^2.5.14

// and a modified version of Adafruit_GFX that
// has a modified text wrap functionality
// in the write function. It adds some more space
// for wrapped text
// based on adafruit/Adafruit GFX Library@^1.12.1

extern Adafruit_SSD1306 _display;
extern Adafruit_SSD1306 _display2;
extern uint8_t swappedDisplays;
extern uint16_t ticks;


void initDisplays();
void initTicks();
void updateTicks();
void clearDisplays();
void refreshDisplays();
void swapDisplays();

void calcAC(uint8_t centerX, uint8_t centerY, float val);
bool isLeftOfLine(int16_t centerX, int16_t centerY, int16_t x, int16_t y);
void plotFirstSec(Adafruit_SSD1306 *display, int16_t xc, int16_t yc, int16_t x, int16_t y, float val);
void plotSecondSec(Adafruit_SSD1306 *display, int16_t xc, int16_t yc, int16_t x, int16_t y, float val);
void plotThirdSec(Adafruit_SSD1306 *display, int16_t xc, int16_t yc, int16_t x, int16_t y, float val);
void plotFourthSec(Adafruit_SSD1306 *display, int16_t xc, int16_t yc, int16_t x, int16_t y, float val);
void plotArcPointsVal(Adafruit_SSD1306 *display, int16_t xc, int16_t yc, int16_t x, int16_t y, float val);
void drawArcVal(Adafruit_SSD1306 *display, int16_t xc, int16_t yc, int16_t r, float val);
void plotArcPoints(Adafruit_SSD1306 *display, int16_t xc, int16_t yc, int16_t x, int16_t y);
void drawArc(Adafruit_SSD1306 *display, int16_t xc, int16_t yc, int16_t r);
void drawArcMeter(Adafruit_SSD1306 *display, int16_t centerX, int16_t centerY, int16_t radius, int16_t thickness, float val);
void drawRectanglePer(Adafruit_SSD1306 *display, uint8_t x, int8_t y, uint8_t w, uint8_t h, float per);

#endif
