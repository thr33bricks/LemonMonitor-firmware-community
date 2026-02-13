/*
 * Project: Lemon Monitor Firmware
 * Author:  Yordan Yordanov
 * Date:    2025-06
 * Github:  https://github.com/thr33bricks/LemonMonitor-firmware-community
 *
 * License: GNU GPL V3 - See LICENSE file for details.
 */

#ifndef PAGE_LOADING_H
#define PAGE_LOADING_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#ifdef BOUNCY_INFO
void initPageLoading();
void pageLoadingBouncyInfo();
#elif defined(BOUNCY)
void pageLoadingBouncy();
#elif defined(MATRIX)
void drawRandom10(Adafruit_SSD1306 *display, uint8_t xOffset);
void pageLoadingMatrix();
#endif

#endif
