/*
 * Project: Lemon Monitor Firmware
 * Author:  Yordan Yordanov
 * Date:    2025-06
 * Github:  https://github.com/thr33bricks/LemonMonitor-firmware-community
 *
 * License: GNU GPL V3 - See LICENSE file for details.
 */

#include "page_loading.h"
#include "display.h"
#include "bitmaps.h"
#include "settings.h"


#ifdef MATRIX
uint16_t lastLoadingTick;
int8_t currLogoCol = 0;

#elif defined(BOUNCY_INFO) || defined(BOUNCY)
uint8_t currLogoX = 0;
uint8_t currLogoY = 0;

#ifdef BOUNCY
int8_t logoDy = 2;
int8_t logoDx = 2;

#elif defined(BOUNCY_INFO)
int8_t logoDy = 1; // 2
int8_t logoDx = 1; // 2


void initPageLoading(){
    uint8_t rndDisp = random(0, 2);
    if(rndDisp == 0)
        swapDisplays();

    currLogoX = random(0, 113);
    currLogoY = random(0, 45);

    _display2.clearDisplay();
    _display2.setTextColor(WHITE, BLACK);

    _display2.setTextSize(2);
    _display2.setCursor(34, 5);
    _display2.print(F("Lemon"));

    _display2.setTextSize(1);
    _display2.setCursor(34, 24);
    _display2.print(F("monitor "));
    _display2.print(F(MODEL));
    _display2.setCursor(22, 44);
    _display2.print(F("firmware: v"));
    _display2.print(FIRMWARE_VERSION/10);
    _display2.print('.');
    _display2.print(FIRMWARE_VERSION%10);
    _display2.setCursor(4, 54);
    _display2.print(F(LINK_APP));
    _display2.display();

    logoDy = 1; // 2
    logoDx = 1; // 2
}

// Bouncy bouncy on one display and info on the other
void pageLoadingBouncyInfo(){
    uint8_t minY = 0;
    uint8_t maxY = 45;
    uint8_t minX = 0;
    uint16_t maxX = 113;

    _display.clearDisplay();
    _display.drawBitmap(currLogoX, currLogoY, Logo,15,19,WHITE);
    _display.display();

    currLogoX += logoDx;
    currLogoY += logoDy;
    if(currLogoX >= maxX || currLogoX <= minX)
        logoDx *= -1;
    if(currLogoY >= maxY || currLogoY <= minY)
        logoDy *= -1;
}
#endif
#endif

#ifdef BOUNCY
// Bouncy bouncy on both displays
void pageLoadingBouncy(){
    uint8_t minY = 0;
    uint8_t maxY = 44;
    uint8_t minX = 0;
    uint16_t maxX = 240;

    // Faster rendering but lags when logo is
    // between the 2 displays
    // if(currLogoX < 128){
    //   display.clearDisplay();
    //   display.drawBitmap(currLogoX, currLogoY, Logo,15,19,WHITE);
    //   display.display();
    // }

    // if(currLogoX > 112){
    //   display2.clearDisplay();
    //   display2.drawBitmap(currLogoX - 128, currLogoY, Logo,15,19,WHITE);
    //   display2.display();
    // }

    _display.clearDisplay();
    if(currLogoX < 129)
        _display.drawBitmap(currLogoX, currLogoY, Logo,15,19,WHITE);
    _display.display();

    _display2.clearDisplay();
    if(currLogoX > 112)
        _display2.drawBitmap(currLogoX - 128, currLogoY, Logo,15,19,WHITE);
    _display2.display();

    currLogoX += logoDx;
    currLogoY += logoDy;
    if(currLogoX >= maxX || currLogoX <= minX)
        logoDx *= -1;
    if(currLogoY >= maxY || currLogoY <= minY)
        logoDy *= -1;
}
#endif

#ifdef MATRIX
// Draws random 0s and 1s in a "The matrix" pattern
void drawRandom10(Adafruit_SSD1306 *display, uint8_t xOffset){
    uint8_t maxRowStart = 4;
    uint8_t minColLen = 2;
    uint8_t maxColLen = 4;

    (*display).setTextSize(1);

    for (uint8_t col = 0; col < 21; ++col){
        uint8_t row = random(0, maxRowStart+1); // start
        uint8_t endRow = random(row + minColLen, row + maxColLen + 2); // end

        for (; row < endRow; ++row){
            uint8_t num = random(0, 2);
            (*display).setCursor(xOffset + col * 6, row * 8);
            (*display).print(num);
        }
    }
}

// Matrix loading page
void pageLoadingMatrix(){
    uint8_t xOffset = 1;
    const __FlashStringHelper *branding = F(" Lemon monitor ");

    if(ticks - lastLoadingTick < 8)
        return;
    lastLoadingTick = ticks;

    _display.clearDisplay();
    drawRandom10(&_display, xOffset);
    _display.setTextSize(2);
    _display.setCursor(xOffset + currLogoCol * 6, 24);
    _display.print(branding);
    _display.display();

    _display2.clearDisplay();
    drawRandom10(&_display2, xOffset);
    _display2.setTextSize(2);
    _display2.setCursor(xOffset + (currLogoCol-20) * 6, 24);
    _display2.print(branding);
    _display2.display();

    currLogoCol+=3;
    if(currLogoCol >= 40)
        currLogoCol = -29;
}
#endif
