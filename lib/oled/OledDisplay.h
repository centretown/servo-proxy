// Copyright (c) 2021 Dave Marsh. See LICENSE.
#ifdef ARDUINO

#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
// Adafruit_I2CDevice.h
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

class OledDisplay
{
private:
    Adafruit_SSD1306 &display;
    uint8_t addr;
    /* data */
    bool active = false;
    uint16_t textColor;

    // color

public:
    OledDisplay(Adafruit_SSD1306 &display, uint8_t addr);
    ~OledDisplay();

    void setup();
    bool isActive() { return active; }

    void drawText(const char *text, uint8_t x = 0, uint8_t y = 0, uint8_t size = 2, uint16_t color = SSD1306_WHITE);
    void drawBitmap();
    void loop();
    void reset();
    int process(const char *buf);
    void start(uint8_t command, char *parms, size_t nparms);

    void dog();
    void dartboard();
    void panda();
    void strip();
};

#endif
