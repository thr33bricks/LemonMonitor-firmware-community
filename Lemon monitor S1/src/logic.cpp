/*
 * Project: Lemon Monitor Firmware
 * Author:  Yordan Yordanov
 * Date:    2025-06
 * Github:  https://github.com/thr33bricks/LemonMonitor-firmware-community
 *
 * License: GNU GPL V3 - See LICENSE file for details.
 */

#include "logic.h"
#include "serial.h"
#include "display.h"
#include "settings.h"
#include "page_loading.h"
#include "page_main.h"

State state;

uint8_t firstPage = 255;
uint8_t lastPage = 255;
int8_t yOffset = Y_OFFSET_START;
uint8_t minPage = 0;
uint16_t pageTicks = 0;

uint8_t currPage = 4;


void initState(){
    state = STATE_LOADING;

    #ifdef BOUNCY_INFO
    initPageLoading();
    #endif
}

void initMain(){
    firstPage = 255;
    lastPage = 255;
    yOffset = Y_OFFSET_START;
    minPage = 0;
    pageTicks = ticks;
    currPage = 4;

    selectNextPage();
    selectNextPage();
}

void updateState(){
    switch (state){
        case STATE_LOADING:
            state = handleLoading();
            break;
        case STATE_SLEEP:
            state = handleSleep();
            break;
        case STATE_MAIN:
            state = handleMain();
            break;
    }
}

void putPage(uint8_t page){
    if(firstPage == 255)
        firstPage = page;
    else if(lastPage == 255)
        lastPage = page;
    else{
        firstPage = lastPage;
        lastPage = page; 
    }
}

void drawPage(uint8_t page, uint8_t yOffset){
    switch (page){
        case 0: // CPU
            drawCpuGpuPage(&_display2, yOffset, getCPUTemp(), getCPULoad(), getCpuName());
            break;
        case 1: // RAM
            drawRamPage(&_display2, yOffset, getRAM(), 0);
            break;
        case 2: // GPU
            drawCpuGpuPage(&_display2, yOffset, getGPUTemp(), getGPULoad(), getGpuName());
            break;
        case 3: // VRAM
            drawRamPage(&_display2, yOffset, getVRAM(), 1);
            break;
        case 4: // All info
            drawAllInfoPage(&_display2, getCPULoad(), getCPUTemp(), getGPULoad(), getGPUTemp());
            break;
        default:
            break;
    }
}

void selectNextPage(){
    uint8_t page = minPage;
    uint8_t pages = 4;
    // 0 => CPU, 1 => RAM, 2 => GPU, 3 => VRAM

    for (; page < pages; ++page){
        if((page == 2 && getGPUTemp() == 2) || 
           (page == 3 && getVRAM() == 0)){
            continue; // skip GPU and VRAM if they are not available
        }
        minPage = page + 1;
        putPage(page);
        break;
    }

    // Rollover
    if(minPage == pages){
        minPage = 0;
        return;
    }

    // If no page was added because we skipped
    if(page == pages){
        minPage = 1;
        putPage(0);
    }
}

void updatePage(){
    switch (getKeyboardAction()){
        case 2:
            // left
            currPage = (currPage == 0) ? 4 : currPage - 1;
            break;
        case 3:
            // right
            currPage = (currPage == 4) ? 0 : currPage + 1;
            break;
        default:
            break;
    }
}

State handleLoading(){
    if(ticks > LOADING_TIME){
        if(ticks > SLEEP_TIME){
            #ifdef BOUNCY_INFO
            if(swappedDisplays)
                swapDisplays();
            #endif

            return STATE_SLEEP;
        }
        else if(isNewPacket()){
            #ifdef BOUNCY_INFO
            if(swappedDisplays)
                swapDisplays();
            #endif

            initMain();
            return STATE_MAIN;
        }
    }

    #ifdef MATRIX
    pageLoadingMatrix();
    #elif defined(BOUNCY)
    pageLoadingBouncy();
    #elif defined(BOUNCY_INFO)
    pageLoadingBouncyInfo();
    #endif

    return STATE_LOADING;
}

State handleSleep(){
    clearDisplays();
    refreshDisplays();

    if(isNewPacket()){
        initMain();
        return STATE_MAIN;
    }

    return STATE_SLEEP;
}

State handleMain(){
    if((uint16_t)(ticks - lastPacketTime) > SLEEP_TIME)
        return STATE_SLEEP;

    _display.clearDisplay();
    drawFpsPage(&_display, 0, getCurrFPS(), getAvgFPS(), get1PerFPS(), getFpsBar(), areDividersEnabled());
    _display.display();


    if(getModeMain() == 2){
        updatePage();
        pageTicks = ticks;
        yOffset = Y_OFFSET_START;

        _display2.clearDisplay();
        drawPage(currPage, 0);
        drawDotsPage(&_display2, currPage);
        _display2.display();

        return STATE_MAIN;
    }

    _display2.clearDisplay();
    drawPage(lastPage, yOffset);
    drawPage(firstPage, yOffset + 64);
    _display2.display();

    if((uint16_t)(ticks - pageTicks) < PAGE_HOLD_TIME)
        return STATE_MAIN;

    yOffset += Y_DELTA;
    if(yOffset >= Y_OFFSET_END){
        yOffset = Y_OFFSET_START;
        pageTicks = ticks;
        selectNextPage();
    }

    return STATE_MAIN;
}
