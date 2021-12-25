// Copyright (c) 2021 Dave Marsh. See LICENSE.
#ifdef ARDUINO

#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define ERR_OLED_OK 0
#define ERR_OLED_NOT_ENOUGH_ARGS 1
#define ERR_OLED_NOT_FOUND 2
#define ERR_OLED_INDEX 3
#define ERR_OLED_BAD_VALUE 4

enum OledCommand
{
    OLED_NOP,       //0
    OLED_RESET,     //1
    OLED_TEXT,      //2
    OLED_DARTBOARD, //3
    OLED_DOG,       //4
    OLED_PANDA,     //5
};

enum IconID
{
    ICON_DARTBOARD,
    ICON_DOG,
    ICON_PANDA,
    ICON_LEDSTRIP,
    ICON_GEARS,

    ICON_COUNT,
};

class OledDisplay
{
private:
    Adafruit_SSD1306 &display;
    uint8_t addr;

    bool active = false;
    uint16_t textColor;

public:
    OledDisplay(Adafruit_SSD1306 &display, uint8_t addr);
    ~OledDisplay();

    void setup();

    bool isActive() { return active; }

    void drawText(const char *text, uint8_t x = 0, uint8_t y = 0, uint8_t size = 2, uint16_t color = SSD1306_WHITE);
    void drawIcon(IconID, int16_t, int16_t);
    void drawMenu(const char *heading, const char *label);
    void drawMenu(const char *heading, const char *label, int16_t value);

    void reset();
    int process(const char *buf);
    void start(uint8_t command, char *parms, size_t nparms);

    void dog() { drawIcon(ICON_DOG, 0, 0); }
    void dartboard() { drawIcon(ICON_DARTBOARD, 0, 0); }
    void panda() { drawIcon(ICON_PANDA, 0, 0); }
    void strip() { drawIcon(ICON_LEDSTRIP, 0, 0); }
};

#endif
