/*
 * Project: Lemon Monitor Firmware
 * Author:  Yordan Yordanov
 * Date:    2025-06
 * Github:  https://github.com/thr33bricks/LemonMonitor-firmware-community
 *
 * License: GNU GPL V3 - See LICENSE file for details.
 */

#include <avr/wdt.h>
#include <Arduino.h>
#include "serial.h"
#include "display.h"
#include "logic.h"


void setup() {
  wdt_disable();
  randomSeed(analogRead(0));
  initSerial();
  initDisplays();
  initState();
}

void loop() {
  updateTicks();
  updateState();
  updateSerial();
}
