// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifndef ARDUINO
#include "NativeStrip.h"
#include "NativeEndPoint.h"

#include "Menu.h"

#define USE_SERVO_LIB
#define USE_LEDSTRIP_LIB


int8_t expanderPins[] = {2, 3, 5, 6};
size_t expanderPinCount = sizeof(expanderPins) / sizeof(expanderPins[0]);

#include "menus.h"


int main(int argc, char **argv)
{
    initServoMenu();
    initLedMenu();

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