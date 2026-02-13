/*
 * Project: Lemon Monitor Firmware
 * Author:  Yordan Yordanov
 * Date:    2025-06
 * Github:  https://github.com/thr33bricks/LemonMonitor-firmware-community
 *
 * License: GNU GPL V3 - See LICENSE file for details.
 */

#ifndef SERIAL_H
#define SERIAL_H

#include <Arduino.h>

extern uint16_t lastPacketTime;

void initSerial();
void updateSerial();
uint8_t dataAvailable(uint8_t ndx, uint8_t writeFullBuffer);
uint8_t waitingData(uint8_t ndx, uint32_t lastByteTime, uint8_t writeFullBuffer);
uint8_t isNewPacket();
uint8_t checkPacket(uint8_t infoByte);
uint16_t getCurrFPS();
uint16_t getAvgFPS();
uint8_t get1PerFPS();
uint8_t getRAM();
uint8_t getVRAM();
uint8_t getCPUTemp();
uint8_t getGPUTemp();
uint8_t getCPULoad();
uint8_t getGPULoad();
uint8_t *getFpsBar();
char *getCpuName();
char *getGpuName();
uint8_t getModeMain();
uint8_t getKeyboardAction();
uint8_t areDividersEnabled();

#endif
