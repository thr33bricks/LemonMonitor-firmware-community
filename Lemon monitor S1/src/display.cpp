/*
 * Project: Lemon Monitor Firmware
 * Author:  Yordan Yordanov
 * Date:    2025-06
 * Github:  https://github.com/thr33bricks/LemonMonitor-firmware-community
 *
 * License: GNU GPL V3 - See LICENSE file for details.
 */

#include "display.h"
#include "settings.h"

Adafruit_SSD1306 _display(128, 64, &Wire, -1);
Adafruit_SSD1306 _display2(128, 64, &Wire, -1);
uint8_t swappedDisplays = 0;
uint16_t ticks = 0;

// Arc visualization variables
float a, c;
int16_t lineX; // y = 0

#ifdef DEBUGGING
uint32_t lastTickTime;
#endif

void initDisplays() {
    if(!_display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C
        Serial.println(F("display 1 allocation failed"));
        for(;;); // loop forever
    }

    uint8_t *screenBuff = _display.getBuffer();
    if(!_display2.begin(SSD1306_SWITCHCAPVCC, 0x3D, screenBuff)) { // Address 0x3D
        Serial.println(F("display 2 allocation failed"));
        for(;;); // loop forever
    }

    clearDisplays();
    refreshDisplays();
    _display.setTextColor(WHITE, BLACK);
    _display2.setTextColor(WHITE, BLACK);
    _display.setTextWrap(false);
    _display2.setTextWrap(false);
    initTicks();
}

void initTicks() {
    ticks = 0;

    #ifdef DEBUGGING
    lastTickTime = millis();
    #endif
}

void updateTicks(){
    #ifdef DEBUGGING
    uint32_t currTime = millis();
    
    Serial.print(F("Loop time: "));
    Serial.println((uint32_t)(currTime - lastTickTime));
    lastTickTime = currTime;
    #endif
    
    ticks++;
}

void clearDisplays(){
    _display.clearDisplay();
}

void refreshDisplays(){
    _display.display();
    _display2.display();
}

void swapDisplays(){
    Adafruit_SSD1306 temp = _display;
    _display = _display2;
    _display2 = temp;
    swappedDisplays = !swappedDisplays;
}

// calculate params for the line defined by the angle
// get a point of the line
// we know y = 0, get x
// y = ax + c
void calcAC(uint8_t centerX, uint8_t centerY, float val){
    a = tan(val * 3.1415926);
    c = centerY - a*centerX;
    lineX = (int16_t)(0.5-c/a);
}

// return true if point is on the left of line
bool isLeftOfLine(int16_t centerX, int16_t centerY, int16_t x, int16_t y){
    long crossProduct = (long)(centerX - lineX) * y - centerY * (long)(x - lineX);
    return crossProduct >= 0 ? true : false;
}

void plotFirstSec(Adafruit_SSD1306 *display, int16_t xc, int16_t yc, int16_t x, int16_t y, float val){
    if(val <= 0.25){
        if(isLeftOfLine(xc, yc, xc - y, yc - x)) (*display).drawPixel(xc - y, yc - x, WHITE);
    }else
        (*display).drawPixel(xc - y, yc - x, WHITE); // 1
}

void plotSecondSec(Adafruit_SSD1306 *display, int16_t xc, int16_t yc, int16_t x, int16_t y, float val){
    if(val > 0.25){
        if(val <= 0.50){
            if(isLeftOfLine(xc, yc, xc - x, yc - y)) (*display).drawPixel(xc - x, yc - y, WHITE);
        }else
            (*display).drawPixel(xc - x, yc - y, WHITE); // 2
    } 
}

void plotThirdSec(Adafruit_SSD1306 *display, int16_t xc, int16_t yc, int16_t x, int16_t y, float val){
    if(val > 0.50){
        if(val <= 0.75){
            if(isLeftOfLine(xc, yc, xc + x, yc - y)) (*display).drawPixel(xc + x, yc - y, WHITE);
        }else
            (*display).drawPixel(xc + x, yc - y, WHITE); // 3
    } 
}

void plotFourthSec(Adafruit_SSD1306 *display, int16_t xc, int16_t yc, int16_t x, int16_t y, float val){
    if(val > 0.75 && isLeftOfLine(xc, yc, xc + y, yc - x))
        (*display).drawPixel(xc + y, yc - x, WHITE); // 4
}

// Plotting eight symmetrical points
void plotArcPointsVal(Adafruit_SSD1306 *display, int16_t xc, int16_t yc, int16_t x, int16_t y, float val) {
    plotFirstSec(display, xc, yc, x, y, val);
    plotSecondSec(display, xc, yc, x, y, val);
    plotThirdSec(display, xc, yc, x, y, val);
    plotFourthSec(display, xc, yc, x, y, val);
}
void drawArcVal(Adafruit_SSD1306 *display, int16_t xc, int16_t yc, int16_t r, float val) {
    int16_t x = 0, y = r;
    int16_t d = 3 - 2 * r; // Initial decision parameter

    // Plot initial points
    plotArcPointsVal(display, xc, yc, x, y, val);

    while (y >= x) {
        x++;

        // Update decision parameter based on its value
        if (d <= 0) {
            d = d + 4 * x + 6;
        } else {
            y--;
            d = d + 4 * (x - y) + 10;
        }

        // Plot points for the current (x, y)
        plotArcPointsVal(display, xc, yc, x, y, val);
    }
}

void plotArcPoints(Adafruit_SSD1306 *display, int16_t xc, int16_t yc, int16_t x, int16_t y){
    (*display).drawPixel(xc - y, yc - x, WHITE); // 1
    (*display).drawPixel(xc - x, yc - y, WHITE); // 2
    (*display).drawPixel(xc + x, yc - y, WHITE); // 3
    (*display).drawPixel(xc + y, yc - x, WHITE); // 4
}

void drawArc(Adafruit_SSD1306 *display, int16_t xc, int16_t yc, int16_t r) {
    int16_t x = 0, y = r;
    int16_t d = 3 - 2 * r; // Initial decision parameter

    // Plot initial points
    plotArcPoints(display, xc, yc, x, y);

    while (y >= x) {
        x++;

        // Update decision parameter based on its value
        if (d <= 0) {
            d = d + 4 * x + 6;
        } else {
            y--;
            d = d + 4 * (x - y) + 10;
        }

        // Plot points for the current (x, y)
        plotArcPoints(display, xc, yc, x, y);
    }
}

void drawArcMeter(Adafruit_SSD1306 *display, int16_t centerX, int16_t centerY, int16_t radius, int16_t thickness, float val){
    if(val < 0.005)
        val = 0.005;
    else if(val > 0.995)
        val = 0.995;

    drawArc(display, centerX, centerY, radius);
    drawArc(display, centerX, centerY, radius - thickness);
    (*display).writeFastHLine(centerX + radius - thickness, centerY, thickness, WHITE); // right line
    (*display).writeFastHLine(centerX - radius, centerY, thickness, WHITE); // left line

    // shows the angle a=of the fill with a line
    #ifdef DEBUGGING
    (*display).drawLine(centerX, centerY, lineX, 0, WHITE);
    #endif
    
    calcAC(centerX, centerY, val);

    for(int16_t i = 2; i < thickness; i+=2)
        drawArcVal(display, centerX, centerY, radius - i, val);
}

void drawRectanglePer(Adafruit_SSD1306 *display, uint8_t x, int8_t y, uint8_t w, uint8_t h, float per){
    uint8_t fillHeight = (uint8_t)(h * per / 100.0 + 0.5);
    fillHeight = ((fillHeight > h) ? h : fillHeight);
    (*display).drawRect(x, y, w, h, WHITE);
    (*display).fillRect(x, y + (h-fillHeight), w, fillHeight, WHITE);
}
