/*
 * Project: Lemon Monitor Firmware
 * Author:  Yordan Yordanov
 * Date:    2025-06
 * Github:  https://github.com/thr33bricks/LemonMonitor-firmware-community
 *
 * License: GNU GPL V3 - See LICENSE file for details.
 *
 * 
 * Serial communication protocol:
 *
 * 00 00 0E => soft reset
 * 00 00 08 => get device
 *
 *          <---RAM--->
 * 00 00 57 00 20 04 01 37 63 2D 01 01 45 01 => normal scrolling, div enabled
 * 00 00 57 00 20 04 01 37 63 2D 01 01 86 01 => normal switch, div disabled
 * 00 00 57 00 20 04 01 37 63 2D 01 01 8A 01 => normal switch, left action, div disabled
 * 00 00 57 00 20 04 01 37 63 2D 01 01 8D 01 => normal switch, right action, div enabled
 *
 * => normal switch, right action, div enabled, with fps bars
 * 00 00 57 00 20 04 01 37 63 2D 01 01 8D 01 01 23 45 67 01 23 45 67 01 23 45 67 01 23 45 67 01 23 45 67 01 23 45 67 01 23 45 67 FD CA 98 76
 *
 *
 * => full-buffer switch, right action, div enabled, with fps bars, with cpu and gpu names
 * Note: has to be sent in 2 parts as the internal serial buffer is 64B only
 *          <---RAM--->                      <-----------------------------------------FPS Bars-------------------------------------------->
 * 00 00 D9 00 20 04 01 37 63 2D 01 01 8D 01 01 23 45 67 01 23 45 67 01 23 45 67 01 23 45 67 01 23 45 67 01 23 45 67 01 23 45 67 FD CA 98 76
 * <--------------------------------Intel Core i5-11400H-----------------------------------------> <-----------------------------------Nvidia GeForce RTX 3060----------------------------------->
 * 49 6E 74 65 6C 20 43 6F 72 65 20 69 35 2D 31 31 34 30 30 48 00 00 00 00 00 00 00 00 00 00 00 00 4E 76 69 64 69 61 20 47 65 46 6F 72 63 65 20 52 54 58 20 33 30 36 30 00 00 00 00 00 00 00 00 00
 */

#include <avr/wdt.h>
#include "serial.h"
#include "settings.h"
#include "display.h"

#define BUFFER 107

uint8_t serialBuffer[BUFFER];
uint8_t newData = 0;
uint16_t lastPacketTime = 0;


void initSerial(){
    Serial.begin(SERIAL_BAUD);
    lastPacketTime = 0;
    newData = 0;
}

void updateSerial(){
    uint32_t lastByteTime = millis();
    uint8_t writeFullBuffer = 0;
    uint8_t recvInProgress = 0;
    uint8_t startByteCount = 0;
    uint8_t ndx = 0;

    while (dataAvailable(ndx, writeFullBuffer) || waitingData(ndx, lastByteTime, writeFullBuffer)){
        int16_t read = Serial.read();
        if(read == -1)
            continue;

        if (recvInProgress){
            lastByteTime = millis();
            serialBuffer[ndx] = (uint8_t)read;
            ndx++;
            
            // In case we try to enter the full-buffer
            // territory without permission
            if (!writeFullBuffer && ndx > 42)
                ndx = 42;
        }
        else if (startByteCount == 2){
            uint8_t res = checkPacket((uint8_t)read);
            if(res == 2){
                writeFullBuffer = 1;
                res = 1;
            }

            recvInProgress = res;
            if(!res)
                startByteCount = 0;
        }
        else if ((uint8_t)read == 0)
            startByteCount++;
        else
            startByteCount = 0;
    }

    if(ndx > 0){
        newData = 1;
        lastPacketTime = ticks;
    }
}

// Data is available and we haven't reached the end of the internal
// serial buffer. Note: end of full data serial buffer is reachable 
// only in full-buffer writing mode
uint8_t dataAvailable(uint8_t ndx, uint8_t writeFullBuffer){
    return (!writeFullBuffer && Serial.available() > 0 && ndx < SERIAL_RX_BUFFER_SIZE);
}

// Some data is received but timeout hasn't passed
// when waiting for the second part of the buffer
uint8_t waitingData(uint8_t ndx, uint32_t lastByteTime, uint8_t writeFullBuffer){
    return (writeFullBuffer && ndx < BUFFER
            && (uint32_t)(millis() - lastByteTime) < SERIAL_TIMEOUT);
}

// Returns: 1 => valid, 0 => invalid, 2 => write full 107B buffer
// (len == 4 && type == 0) => Print S/N, Model, Firmware
// (len == 7 && type == 0) => Reset
// (len == 43 && type == 1) => Normal serial write
// (len == 108 && type == 1) => Full-buffer serial write
uint8_t checkPacket(uint8_t infoByte){
    uint8_t len = infoByte >> 1;
    uint8_t type = infoByte & 1;
    uint8_t res = 0;

    if(len == 4 && type == 0){
        Serial.print(F("<"));
        Serial.print(F(SERIAL_NUMBER));
        Serial.print(F(","));
        Serial.print(F(MODEL));
        Serial.print(F(","));
        Serial.print(FIRMWARE_VERSION);
        Serial.print(F(">"));
    }
    else if(len == 7 && type == 0){
        wdt_enable(WDTO_15MS);
        while (1) {}
    }
    else if(len == 43 && type == 1)
        res = 1;
    else if(len == 108 && type == 1)
        res = 2;

    return res;
}

uint8_t isNewPacket(){
    uint8_t res = newData;
    if(newData)
        newData = 0;
    return res;
}

uint16_t getCurrFPS(){
    uint16_t currFps = ((uint16_t)serialBuffer[0] << 8) | serialBuffer[1];
    return (currFps>>5);
}

uint16_t getAvgFPS(){
    uint16_t avgFps = (((uint16_t)(serialBuffer[1] & 0b00011111)) << 6) | (serialBuffer[2]>>2);
    return avgFps;
}

uint8_t get1PerFPS(){
    uint16_t onePerLow = (((uint16_t)serialBuffer[2] & 3) << 8) | (serialBuffer[3]);
    return onePerLow;
}

uint8_t getRAM(){
    return serialBuffer[4];
}

uint8_t getCPUTemp(){
    return serialBuffer[5];
}

uint8_t getCPULoad(){
    return serialBuffer[6];
}

uint8_t getGPUTemp(){
    return serialBuffer[7];
}

uint8_t getGPULoad(){
    return serialBuffer[8];
}

// 1 is scrolling, 2 is keyboard action mode
uint8_t getModeMain(){
    uint8_t modeMain = ((serialBuffer[9] & 0b11000000) >> 6);
    return modeMain;
}

// 1 is nothing, 2 is left, 3 is right
uint8_t getKeyboardAction(){
    uint8_t kbdAction = ((serialBuffer[9] & 0b00111100) >> 2);
    serialBuffer[9] = (serialBuffer[9] & 0b11000011) | 0b00000100;
    return kbdAction;
}

// 1 is enabled, 2 is disabled
uint8_t areDividersEnabled(){
    return (serialBuffer[9] & 3);
}

uint8_t getVRAM(){
    return serialBuffer[10];
}

uint8_t *getFpsBar(){
    return &serialBuffer[11];
}

char *getCpuName(){
    return (char *)&serialBuffer[43];
}

char *getGpuName(){
    return (char *)&serialBuffer[75];
}
