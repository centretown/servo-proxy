// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifndef ARDUINO
#include "Menu.h"
#include "NativeMenuReader.h"
#include "NativeMenuWriter.h"

#define USE_SERVO_LIB
#define USE_LEDSTRIP_LIB

void servoParmsEndPoint(Menu *menu)
{
}

void servoEndPoint(Menu *menu)
{
}

int8_t expanderPins[] = {2, 3, 5, 6};

#include "menus.h"

int main(int argc, char **argv)
{
    initServoMenu();
    initLedMenu();

    NativeMenuReader reader;
    NativeMenuWriter writer;

    Menu::SetReader(&reader);
    Menu::SetWriter(&writer);
    Menu::setup();
    while (true)
    {
        Menu::loop();
    }
}

#endif