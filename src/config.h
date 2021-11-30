// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#define USE_SERVO_LIB

#define USE_LEDSTRIP_LIB

#define USE_ROTARY_LIB

// #define USE_OLED_LIB

// #define USE_TOUCH_LIB

#include "Menu.h"

void show(Menu *menu)
{
    if (menu == NULL)
    {
        Serial.println("null");
        return;
    }
    uint8_t index = menu->Index();
    if (index != UNSELECTED)
    {
        Menu *node = menu->nodes[index];
        Serial.print(menu->Label());
        Serial.print(": ");
        Serial.println(node->Label());
    }
}

void endPoint(Menu *menu)
{
    char buf[80];
    snprintf(buf, sizeof(buf), "endPoint <%s i=%u l=%u c=%u>",
             menu->Label(), menu->Index(), menu->Level(), menu->Count());
    // printf("%s\n", buf);
    Serial.println(buf);
    uint8_t level = menu->Level();

    for (uint8_t i = 0; i < level; i++)
    {
        menu = menu->Ancestor(i);
        snprintf(buf, sizeof(buf), "endPoint <%s i=%u l=%u c=%u>",
                 menu->Label(), menu->Index(), menu->Level(), menu->Count());
        Serial.println(buf);
        // printf("%s\n", buf);
    }
}

//////////////////////////////
#if defined(USE_SERVO_LIB)
#include "ServoServe.h"
#if defined(USE_PCA9685_SERVO_EXPANDER)
#if defined(ARDUINO_SAM_DUE)
ServoEasing Servo1(PCA9685_DEFAULT_ADDRESS, &Wire1);
ServoEasing Servo2(PCA9685_DEFAULT_ADDRESS, &Wire1);
ServoEasing Servo3(PCA9685_DEFAULT_ADDRESS, &Wire1);
ServoEasing Servo4(PCA9685_DEFAULT_ADDRESS, &Wire1);
#else
ServoEasing Servo1(PCA9685_DEFAULT_ADDRESS, &Wire);
ServoEasing Servo2(PCA9685_DEFAULT_ADDRESS, &Wire);
ServoEasing Servo3(PCA9685_DEFAULT_ADDRESS, &Wire);
ServoEasing Servo4(PCA9685_DEFAULT_ADDRESS, &Wire);
#endif
#else
ServoEasing Servo1;
ServoEasing Servo2;
ServoEasing Servo3;
ServoEasing Servo4;
#endif

ServoEasing *servos[] = {&Servo1, &Servo2, &Servo3, &Servo4};
ServoEasing **pServos = servos;
// Only works on pin 2, 3, 5, 6, 7, 8, 44, 45 and 46 on Arduino Mega!
int8_t expanderPins[] = {2, 3, 5, 6};
ServoServe servoServe(pServos, expanderPins,
                      sizeof(expanderPins) / sizeof(expanderPins)[0]);

// menu items
Menu servoMenu("Servos");
uint8_t servoPos = 0;
uint8_t angle = 0;
uint8_t speed = 100;
uint8_t type = 180;

void servoEndPoint(Menu *menu)
{
    uint8_t command = menu->Ancestor(1)->Index() + 1;
    uint8_t index = menu->Ancestor(2)->Index();
    // char buf[80];
    // snprintf(buf, sizeof(buf), "led <%s i=%u %u %u %u %u>",
    //          menu->Label(), index, command, angle, speed, type);
    // Serial.println(buf);
    angle = (angle == 180) ? 0 : 180;
    type = (type == 0) ? 180 : 0;
    // speed = 100;
    servoServe.start(index, command, angle, speed, type);
}

void initServoMenu()
{
    char title[16];
    rootMenu.Add(&servoMenu);
    for (unsigned servoNum = 0; servoNum < 4; servoNum++)
    {
        snprintf(title, sizeof(title), "Servo-%u", servoNum + 1);
        Menu *sub = servoMenu.Add(new Menu(title));
        sub->Add(new Menu("Home", servoEndPoint));
        sub->Add(new Menu("Move", servoEndPoint));
        sub->Add(new Menu("Ease", servoEndPoint));
        sub->Add(new Menu("Test", servoEndPoint));
        sub->Add(new Menu("Stop", servoEndPoint));
        sub->Add(new Menu("Exit"));
    }
    servoMenu.Add(new Menu("Exit"));
}

#endif
//////////////////////////////

//////////////////////////////
#if defined(USE_OLED_LIB)
#include "OledDisplay.h"
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// UNO:       A4(SDA), A5(SCL)
// MEGA 2560: 20(SDA), 21(SCL)
// LEONARDO:   2(SDA),  3(SCL), ...
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 32    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
OledDisplay oled(display, SCREEN_ADDRESS);
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
        Menu *node = menu->nodes[index];
        oled.drawMenu(menu->Label(), node->Label());
    }
}
#endif
//////////////////////////////

//////////////////////////////
#if defined(USE_LEDSTRIP_LIB)
#include "LedStrips.h"

#ifdef __AVR__
#include <avr/power.h>
#endif

#define LEDSTRIP_PIN11 11
#define LEDSTRIP_PIN12 12
#define LEDSTRIP_PIN13 13

Adafruit_NeoPixel strip19(19, LEDSTRIP_PIN11, NEO_BRG + NEO_KHZ800);
Adafruit_NeoPixel strip4(4, LEDSTRIP_PIN12, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip8(8, LEDSTRIP_PIN13, NEO_GRB + NEO_KHZ800);

// Adafruit_NeoPixel strip16 = Adafruit_NeoPixel(7, LEDSTRIP_PIN16, NEO_BRG + NEO_KHZ800);
LedSegment segs[] = {
    LedSegment(0, 3),   //0 4 led indicators
    LedSegment(4, 30),  //1 8+19
    LedSegment(4, 11),  //2 8
    LedSegment(12, 30), //3 19 a
    LedSegment(12, 30), //4 19 b
};

Adafruit_NeoPixel *strips[] = {
    &strip4,
    &strip8,
    &strip19,
};

LedStrips led(strips, sizeof(strips) / sizeof(strips[0]),
              segs, sizeof(segs) / sizeof(segs[0]));

Menu ledMenu("LED Strips");

void ledEndPoint(Menu *menu)
{
    uint8_t command = menu->Ancestor(1)->Index() + 1;
    uint8_t index = menu->Ancestor(2)->Index() + 1;
    // char buf[80];
    // snprintf(buf, sizeof(buf), "led <%s i=%u %u>",
    //          menu->Label(), index, command);
    // Serial.println(buf);
    unsigned parms[4] = {127, 100, 35, 50};
    segs[index].start(command, parms, sizeof(parms));
}

void initLedMenu()
{
    char title[16];
    rootMenu.Add(&ledMenu);
    for (unsigned segNum = 0; segNum < 4; segNum++)
    {
        snprintf(title, sizeof(title), "Strip-%u", segNum + 1);
        Menu *sub = ledMenu.Add(new Menu(title));
        sub->Add(new Menu("Reset", ledEndPoint));
        sub->Add(new Menu("Solid", ledEndPoint));
        sub->Add(new Menu("Blink", ledEndPoint));
        sub->Add(new Menu("Wipe", ledEndPoint));
        sub->Add(new Menu("Cycle", ledEndPoint));
        sub->Add(new Menu("Rainbow", ledEndPoint));
        sub->Add(new Menu("Chase", ledEndPoint));
        sub->Add(new Menu("CycleChase", ledEndPoint));
        sub->Add(new Menu("Exit"));
    }
    ledMenu.Add(new Menu("Exit"));
}

#endif
//////////////////////////////

//////////////////////////////
#if defined(USE_TOUCH_LIB)
#include "TouchSensor.h"

#define TOUCH_SENSOR_PIN 44
TouchSensor touch(TOUCH_SENSOR_PIN);

void touchMenu()
{
    TouchState state = touch.GetState();
    if (state == TOUCH_TAP)
    {
        // Serial.println("next");
        Menu::Next();
    }
    else if (state == TOUCH_HOLD)
    {
        // Serial.println("select");
        Menu::Select();
    }
    else
    {
        return;
    }
    Menu *menu = Menu::Current();

#if defined(USE_OLED_LIB)
    oledShow(menu);
#else
    show(menu);
#endif
}

#endif
//////////////////////////////

//////////////////////////////
#if defined(USE_ROTARY_LIB)
#include "Rotary.h"

#define ROTARY_CLOCK 21
#define ROTARY_DATA 20
#define ROTARY_BUTTON 44

Rotary rotary(ROTARY_CLOCK, ROTARY_DATA, ROTARY_BUTTON);

void rotaryMenu()
{
    RotaryState state = rotary.GetState();
    if (state == ROTARY_NOP)
    {
        return;
    }

    if (state == ROTARY_COUNTER_CLOCKWISE)
    {
        Menu::Previous();
    }
    else if (state == ROTARY_CLOCKWISE)
    {
        Menu::Next();
    }
    else if (state == ROTARY_CLICK)
    {
        Menu::Select();
    }
    else
    {
        return;
    }

    Menu *menu = Menu::Current();
#if defined(USE_OLED_LIB)
    oledShow(menu);
#else
    show(menu);
#endif
}
#endif
