// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "Menu.h"

#ifdef ARDUINO
#include "freeMemory.h"

#ifdef USE_OLED_LIB
#include "OledWriter.h"
OledWriter oledWriter(oled);
#endif //USE_OLED_LIB

#ifdef USE_TOUCH_LIB
#include "TouchReader.h"
TouchReader touchReader(touch);
#endif // USE_TOUCH_LIB

#ifdef USE_ROTARY_LIB
#include <RotaryReader.h>
RotaryReader rotaryReader(rotary);
#endif // USE_ROTARY_LIB

#else

#include "NativeReader.h"
#include "NativeWriter.h"
NativeReader nativeReader;
NativeWriter nativeWriter;
void printFree()
{
}
void printMenuParams(Menu *menu)
{
    printf("l=%u c=%u i=%u r=%u s=%u\n", menu->Length(), menu->Count(),
           menu->Index(), menu->Range(), menu->Sequence());
}
#endif // ARDUINO

// menu items

#ifdef USE_SERVO_LIB
static uint8_t servoAngle = 0;
static uint8_t servoSpeed = 100;
static uint8_t servoType = 180;

#ifdef ARDUINO
void servoParmsEndPoint(Menu *menu)
{
}
void servoEndPoint(Menu *menu)
{
    uint8_t command = menu->Ancestor(1)->Index() + SERVO_HOME;
    uint8_t index = menu->Ancestor(2)->Index();
    servoAngle = (servoAngle == 180) ? 0 : 180;
    servoType = (servoType == 0) ? 180 : 0;

    servoServe.start(index, command, servoAngle, servoSpeed, servoType);
}
#else
void servoParmsEndPoint(Menu *menu)
{
}

void servoEndPoint(Menu *menu)
{
}
#endif // ARDUINO

const char *servoParmsText[] = {"Angle", "Speed", "Option"};
const size_t servoParmsCount = sizeof(servoParmsText) / sizeof(servoParmsText[0]);

void addServoParmsMenu(Menu *menu)
{
    addEndpoints(menu, servoParmsText, servoParmsCount, servoParmsEndPoint);
    addExit(menu);
}

const char *servoSetupText[] = {"Move", "Ease", "Test"};
const size_t servoSetupLength = sizeof(servoSetupText) / sizeof(servoSetupText[0]);
const char *servoSubText[] = {"Home", "Move", "Ease", "Test", "Stop"};
const size_t servoSubLength = sizeof(servoSubText) / sizeof(servoSubText[0]);

void addServoSubMenu(Menu *menu)
{
    size_t servoCount = expanderPinCount;
    // add 1 for setup, 1 for exit
    Menu *sub = menu->Add(new Menu("Servo", servoSubLength + 2, (uint8_t)servoCount));
    addEndpoints(sub, servoSubText, servoSubLength, servoEndPoint);
    Menu *setup = sub->Add(new Menu("Setup", servoSetupLength + 1));
    addMenus(setup, servoSetupText, servoSetupLength, servoParmsCount + 1,
             addServoParmsMenu);
    addExit(sub);
}

void initServoMenu()
{
    Menu *servosMenu = rootMenu.Add(new Menu("Servos", 2));
    addServoSubMenu(servosMenu);
    printFree();
    addExit(servosMenu);
    printFree();
}
#endif // USE_SERVO_LIB

#ifdef USE_LEDSTRIP_LIB
#ifdef ARDUINO
LedEndPoint point(led);
#else
uint8_t segs[5] = {0};
NativeEndPoint point;
#endif // ARDUINO

void ledParmsEndPoint(Menu *menu)
{
    // uint8_t parameter = menu->Ancestor(1)->Index();
    // point.SetParameter(parameter);
    // point.start();
}

void ledEndPoint(Menu *menu)
{
    uint8_t parameter = menu->Index();
    // uint8_t command = menu->Ancestor(1)->Index() + STRIP_FIRST;
    uint8_t command = menu->Ancestor(1)->Index() + 1;
    uint8_t index = menu->Ancestor(2)->Index();
    menu->SetPoint(&point);
    point.setup(index, command, parameter);
    point.start();
}

const char *ledParmsText[] = {"Red", "Green", "Blue", "Repeat", "Delay"};
const size_t ledParmsCount = sizeof(ledParmsText) / sizeof(ledParmsText[0]);
const char *ledSubText[] = {"Solid", "Blink", "Wipe", "Cycle",
                            "Rainbow", "Chase", "CycleChase", "Xmas", "Reset"};
const size_t ledSubLength = sizeof(ledSubText) / sizeof(ledSubText[0]);

void addLedSubMenu(Menu *menu)
{
    size_t segCount = sizeof(segs) / sizeof(segs[0]);
    // add 1 for setup, 1 for exit
    Menu *sub = menu->Add(new Menu("Strip", ledSubLength + 2, segCount));
    addEndpoints(sub, ledSubText, ledSubLength,
                 ledParmsText, ledParmsCount,
                 ledEndPoint, ledParmsEndPoint);
    addExit(sub);
}

void initLedMenu()
{
    // add item for exit
    Menu *ledMenu = rootMenu.Add(new Menu("LED Strips", 2));
    addLedSubMenu(ledMenu);
    printFree();
    addExit(ledMenu);
    printFree();
}

#endif // USE_LEDSTRIP_LIB
