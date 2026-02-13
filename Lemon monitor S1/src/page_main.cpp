/*
 * Project: Lemon Monitor Firmware
 * Author:  Yordan Yordanov
 * Date:    2025-06
 * Github:  https://github.com/thr33bricks/LemonMonitor-firmware-community
 *
 * License: GNU GPL V3 - See LICENSE file for details.
 */

#include "page_main.h"
#include "display.h"
#include "bitmaps.h"
#include "settings.h"


void drawFpsPage(Adafruit_SSD1306 *display, int8_t yOffset, uint16_t currFps, uint16_t avgFps, uint8_t oneLowFps, uint8_t* fpsBar, uint8_t divEn){
  (*display).setTextSize(2);
  (*display).setCursor(7, yOffset+1);
  (*display).print(F("FPS"));
  (*display).setCursor(59, yOffset+1);
  (*display).print(F("AVG"));
  (*display).setCursor(104, yOffset+1);
  (*display).print(F("1%"));

  // Current FPS
  printNum4(display, 1, 22 + yOffset, currFps);

  // Average FPS
  printNum4(display, 53, 22 + yOffset, avgFps);

  // 1 percent low FPS
  printNum2(display, 105, 22 + yOffset, oneLowFps);

  // Dividers
  if(divEn == 1){
    (*display).drawFastVLine(49, 3 + yOffset, 32, WHITE);
    (*display).drawFastVLine(101, 3 + yOffset, 32, WHITE);
  }

  // Fps bars
  for (uint8_t i = 0; i < 64; ++i){
    uint8_t val = ((i%2==0) ? (fpsBar[i/2] >> 4) : (fpsBar[i/2] & 0x0F));
    (*display).drawFastVLine(i*2, 47+(16-val) + yOffset, val, WHITE);
    (*display).drawFastVLine(i*2 + 1, 47+(16-val) + yOffset, val, WHITE);
  }
}

void drawCpuGpuPage(Adafruit_SSD1306 *display, int8_t yOffset, uint8_t temp, uint8_t load, char* entityName){
  (*display).setTextSize(1);
  (*display).setTextWrap(true);
  (*display).setCursor(1, 5 + yOffset);
  (*display).print(entityName);
  (*display).setTextWrap(false);

  // Temp
  (*display).setTextSize(2);
  (*display).setCursor(17, 30 + yOffset);
  (*display).print(temp);
  (*display).setTextSize(1);
  (*display).print(F("o"));

  // Thermometer
  (*display).drawBitmap(0,25+yOffset,Thermometer,16,26,WHITE); // (x,y,name,width,height,colour)
  // percent = ((float)(temp - 40) / (110 - 40)) * 100; // Max 110, min 40
  drawRectanglePer(display, 5, 29 + yOffset, 5, 10, ((float)(((temp < 40) ? 40 : temp) - 40)) * 1.43f);
  (*display).drawFastHLine(7,29 + yOffset, 2, BLACK);

  // Load
  drawRectanglePer(display, 62, 25 + yOffset, 15, 26, load);
  (*display).setTextSize(2);
  (*display).setCursor(83, 30 + yOffset);
  (*display).print(load);
  (*display).setTextSize(1);
  (*display).print(F("%"));
}

void drawRamPage(Adafruit_SSD1306 *display, int8_t yOffset, uint8_t ramPer, uint8_t type){
  (*display).setTextSize(2);

  if(type == 0){
    (*display).setCursor(47, yOffset);
    (*display).print(F("RAM"));
  }
  else{
    (*display).setCursor(41, yOffset);
    (*display).print(F("VRAM"));
  }

  drawArcMeter(display, 63, 57 + yOffset, 41, 12, ((float)ramPer)/100.0f);

  (*display).setTextSize(1);
  if(ramPer<10){
    (*display).setCursor(58, 45 + yOffset);
  }
  else if(ramPer<100){
    (*display).setCursor(55, 45 + yOffset);
  }
  else if(ramPer==100){
    (*display).setCursor(51, 45 + yOffset);
  }
  (*display).print(ramPer);
  (*display).print(F("%"));
}

void drawAllInfoPage(Adafruit_SSD1306 *display, uint8_t cpuLoad, uint8_t cpuTemp, uint8_t gpuLoad, uint8_t gpuTemp){
  (*display).clearDisplay();
  (*display).setTextWrap(false);
  (*display).setTextColor(WHITE);
  // CPU Load
  (*display).setTextSize(1);
  (*display).setCursor(0, 0);
  (*display).print(F("CPU Load: "));

  // CPU Temp
  (*display).setCursor(70, 0);
  (*display).print(F("CPU Temp: "));

  // GPU Load
  (*display).setCursor(0, 33);
  (*display).print(F("GPU Load: "));

  // GPU Temp
  (*display).setCursor(70, 33);
  (*display).print(F("GPU Temp: "));

  // CPU Load
  (*display).setTextSize(2);
  (*display).setCursor(0, 12);
  (*display).print(cpuLoad);
  (*display).setTextSize(1);
  (*display).print(F("%"));

  // CPU Temp
  (*display).setTextSize(2);
  (*display).setCursor(69, 12);
  (*display).print(cpuTemp);
  (*display).setTextSize(1);
  (*display).print(F("o"));

  // GPU Load
  (*display).setTextSize(2);
  (*display).setCursor(0, 43);
  (*display).print(gpuLoad);
  (*display).setTextSize(1);
  (*display).print(F("%"));

  // GPU Temp
  (*display).setTextSize(2);
  (*display).setCursor(69, 43);
  (*display).print(gpuTemp);
  (*display).setTextSize(1);
  (*display).print(F("o"));
}

void drawDotsPage(Adafruit_SSD1306 *display, uint8_t page){
  (*display).drawCircle(51, 61, 2, WHITE);
  (*display).drawCircle(57, 61, 2, WHITE);
  (*display).drawCircle(63, 61, 2, WHITE);
  (*display).drawCircle(69, 61, 2, WHITE);
  (*display).drawCircle(75, 61, 2, WHITE);

  if(page == 4)
    (*display).fillCircle(51, 61, 2, WHITE);
  else if(page == 0)
    (*display).fillCircle(57, 61, 2, WHITE);
  else if(page == 1)
    (*display).fillCircle(63, 61, 2, WHITE);
  else if(page == 2)
    (*display).fillCircle(69, 61, 2, WHITE);
  else if(page == 3)
    (*display).fillCircle(75, 61, 2, WHITE);
}

void printNum4(Adafruit_SSD1306 *display, uint8_t x, int8_t y, uint16_t num){
  if(num == 1){
    (*display).setCursor(x+12, y);
    (*display).print(NO_FPS);
    return;
  }
  else if(num < 10)
    x += 18;
  else if(num < 100)
    x += 12;
  else if(num < 1000)
    x += 6;

  (*display).setCursor(x, y);
  (*display).print(num);
}

void printNum2(Adafruit_SSD1306 *display, uint8_t x, int8_t y, uint8_t num){
  if(num == 1){
    (*display).setCursor(x, y);
    (*display).print(NO_FPS);
    return;
  }
  else if(num < 10)
    x += 5;

  (*display).setCursor(x, y);
  (*display).print(num);
}
