/*
 * Project: Lemon Monitor Firmware
 * Author:  Yordan Yordanov
 * Date:    2025-06
 * Github:  https://github.com/thr33bricks/LemonMonitor-firmware-community
 *
 * License: GNU GPL V3 - See LICENSE file for details.
 */

#ifndef LOGIC_H
#define LOGIC_H

#include <Arduino.h>

enum _State{
    STATE_LOADING,
    STATE_SLEEP,
    STATE_MAIN,
};
typedef uint8_t State;

void initState();
void initMain();
void updateState();
void putPage(uint8_t page);
void drawPage(uint8_t page, uint8_t yOffset);
void selectNextPage();
void updatePage();
State handleLoading();
State handleSleep();
State handleMain();

#endif
