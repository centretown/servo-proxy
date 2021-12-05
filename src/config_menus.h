// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

//////////////////////////////
#ifdef ARDUINO
#include "freeMemory.h"
#else
void printFree()
{
}
void printMenuParams(Menu *menu)
{
    printf("l=%u c=%u i=%u r=%u s=%u\n", menu->Length(), menu->Count(),
           menu->Index(), menu->Range(), menu->Sequence());
}
#endif
//////////////////////////////

#include "Menu.h"

//////////////////////////////
#if defined(USE_OLED_LIB)
uint8_t pic = 0;
bool blinking = false;
void oledShow(Menu *menu)
{
    if (menu == NULL)
    {
        oled.drawText("null");
        return;
    }
    uint8_t index = menu->Index();
    if (index != UNSELECTED)
    {
        // IconID id = (Menu::Root()->Index() == 0) ? ICON_LEDSTRIP : ICON_GEARS;
        Menu *node = menu->Selection();
        oled.drawMenu(menu->Label(), node->Label());
    }
}
#endif
//////////////////////////////

void show(Menu *menu)
{
#if defined(USE_OLED_LIB)
    oledShow(menu);
#else
    Menu *node = menu->Selection();
#if defined(ARDUINO)
    Serial.print(Menu::Path());
    Serial.println(node->Label());
#else
    printMenuParams(menu);
    printMenuParams(node);
    printf("%s%s\n", menu->Path(), node->Label());
#endif
#endif
}

//////////////////////////////
#if defined(USE_ROTARY_LIB)
void rotaryMenu()
{
    RotaryState state = rotary.GetState();
    switch (state)
    {
    case ROTARY_COUNTER_CLOCKWISE:
        Menu::Previous();
        break;

    case ROTARY_CLOCKWISE:
        Menu::Next();
        break;

    case ROTARY_CLICK:
        Menu::Select();
        break;

    default:
        return;
    }
    show(Menu::Current());
}
#endif
//////////////////////////////

//////////////////////////////
#if defined(USE_TOUCH_LIB)
void touchMenu()
{
    TouchState state = touch.GetState();
    switch (state)
    {
    case TOUCH_TAP:
        Menu::Next();
        break;
    case TOUCH_HOLD:
        Menu::Select();
        break;
    default:
        return;
    }
    show(Menu::Current());
}
#endif
//////////////////////////////

// menu items

//////////////////////////////
#if defined(USE_SERVO_LIB)

static uint8_t servoPos = 0;
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
#endif

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
    size_t servoCount = sizeof(expanderPins) / sizeof(expanderPins[0]);
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

#endif
//////////////////////////////

//////////////////////////////
#if defined(USE_LEDSTRIP_LIB)

#if defined(ARDUINO)
void ledParmsEndPoint(Menu *menu)
{
}
void ledEndPoint(Menu *menu)
{
    uint8_t command = menu->Ancestor(1)->Index() + STRIP_SOLID;
    uint8_t index = menu->Ancestor(2)->Index();
    unsigned parms[4] = {127, 100, 35, 50};
    segs[index].start(command, parms, sizeof(parms));
}
#else
uint8_t segs[5] = {0};
void ledParmsEndPoint(Menu *menu)
{
    printf("%s\n", menu->Label());
}
void ledEndPoint(Menu *menu)
{
    printf("%s\n", menu->Label());
}
#endif

const char *ledParmsText[] = {"Red", "Green", "Blue", "Wait", "Duration"};
const size_t ledParmsCount = sizeof(ledParmsText) / sizeof(ledParmsText[0]);

void addLedParmsMenu(Menu *menu)
{
    addEndpoints(menu, ledParmsText, ledParmsCount, ledParmsEndPoint);
    addExit(menu);
}

const char *ledSetupText[] = {"Solid", "Blink", "Wipe", "Cycle",
                              "Rainbow", "Chase", "CycleChase"};
const size_t ledSetupLength = sizeof(ledSetupText) / sizeof(ledSetupText[0]);
const char *ledSubText[] = {"Solid", "Blink", "Wipe", "Cycle",
                            "Rainbow", "Chase", "CycleChase", "Reset"};
const size_t ledSubLength = sizeof(ledSubText) / sizeof(ledSubText[0]);

void addLedSubMenu(Menu *menu)
{
    size_t segCount = sizeof(segs) / sizeof(segs[0]);
    // add 1 for setup, 1 for exit
    Menu *sub = menu->Add(new Menu("Strip", ledSubLength + 2, segCount));
    addEndpoints(sub, ledSubText, ledSubLength, ledEndPoint);
    Menu *setup = sub->Add(new Menu("Setup", ledSetupLength + 1));
    addMenus(setup, ledSetupText, ledSetupLength, ledParmsCount + 1,
             addLedParmsMenu);
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
#endif
//////////////////////////////
