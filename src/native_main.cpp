// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifndef ARDUINO

#include "Menu.h"
#include "LedMenus.h"
#include "NativeReader.h"
#include "NativeWriter.h"
#include "LedSegment.h"

#define USE_SERVO_LIB
#define USE_LEDSTRIP_LIB

#include <time.h>
uint64_t millis()
{
    static uint64_t currentMillis = 0;
    currentMillis += 1000;
    return currentMillis;
}

void printFree() {}

LedSegment segs[] = {
    LedSegment(0, 9),
    LedSegment(10, 19),
    LedSegment(20, 29),
    LedSegment(30, 39),
};
size_t ledSegmentCount = 4;

LedMenus ledMenus(segs, ledSegmentCount);
NativeReader nativeReader;
NativeWriter nativeWriter;

int main(int argc, char **argv)
{
    ledMenus.initLedMenu(ledSegmentCount);
    Menu::SetReader(&nativeReader);
    Menu::SetWriter(&nativeWriter);
    Menu::setup();
    while (true)
    {
        Menu::loop();
    }
    return 0;
}

#endif