/*
 * Project: Lemon Monitor Firmware
 * Author:  Yordan Yordanov
 * Date:    2025-06
 * Github:  https://github.com/thr33bricks/LemonMonitor-firmware-community
 *
 * License: GNU GPL V3 - See LICENSE file for details.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

// Benchmarks:
// While loading: whole loop takes ~40ms
// => TICK must be slightly less than 40ms
// but we still use 40ms to calculate ticks
// as this is the loop time

// Serial
// #define DEBUGGING
#define SERIAL_BAUD 115200
#define SERIAL_TIMEOUT 1000 // 1s

// Timings
#define TICK 37 // 37 ms // Maybe remove?
#define SLEEP_TIME 750 // 750 ticks = 30 sec
#define LOADING_TIME 125 // 125 ticks = 5 sec

// Pages
#define PAGE_HOLD_TIME 25
#define Y_OFFSET_START -61
#define Y_OFFSET_END 3
#define Y_DELTA 4 // 1, 2, 4

//Loading (select one)
//#define MATRIX
#define BOUNCY_INFO
//#define BOUNCY

// FPS
#define NO_FPS F("--")

// DEVICE
#define SERIAL_NUMBER "0"
#define MODEL "S1"
#define FIRMWARE_VERSION 10 // 1.0
#define LINK_APP "lemon.yourdan.uk/app"

#endif
