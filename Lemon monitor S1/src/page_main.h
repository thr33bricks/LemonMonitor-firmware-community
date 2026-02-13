/*
 * Project: Lemon Monitor Firmware
 * Author:  Yordan Yordanov
 * Date:    2025-06
 * Github:  https://github.com/thr33bricks/LemonMonitor-firmware-community
 *
 * License: GNU GPL V3 - See LICENSE file for details.
 */

#ifndef PAGE_MAIN_H
#define PAGE_MAIN_H

#include <Adafruit_SSD1306.h>

void drawFpsPage(Adafruit_SSD1306 *display, int8_t yOffset, uint16_t currFps, uint16_t avgFps, uint8_t oneLowFps, uint8_t* fpsBar, uint8_t divEn);
void drawCpuGpuPage(Adafruit_SSD1306 *display, int8_t yOffset, uint8_t temp, uint8_t load, char* entityName);
void drawRamPage(Adafruit_SSD1306 *display, int8_t yOffset, uint8_t ramPer, uint8_t type);
void drawAllInfoPage(Adafruit_SSD1306 *display, uint8_t cpuLoad, uint8_t cpuTemp, uint8_t gpuLoad, uint8_t gpuTemp);
void drawDotsPage(Adafruit_SSD1306 *display, uint8_t page);
void printNum4(Adafruit_SSD1306 *display, uint8_t x, int8_t y, uint16_t num);
void printNum2(Adafruit_SSD1306 *display, uint8_t x, int8_t y, uint8_t num);

#endif
