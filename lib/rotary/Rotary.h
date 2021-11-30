// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifdef ARDUINO

#pragma once
#include <Arduino.h>

typedef uint8_t RotaryState;

#define ROTARY_NOP 0
#define ROTARY_CLOCKWISE 0x01
#define ROTARY_COUNTER_CLOCKWISE 0x02
#define ROTARY_CLICK 0x04

class Rotary
{
private:
    uint8_t clockPin = 21;
    uint8_t dataPin = 20;
    uint8_t buttonPin = 44;
    int lastClockState = LOW;
    int lastButtonState = LOW;
    uint8_t counter = 0;
    RotaryState state = ROTARY_NOP;
    uint64_t lastTime = 0;
    uint64_t lastButtonClick = 0;

public:
    Rotary(uint8_t c = 21, uint8_t d = 20, uint8_t b = 44);
    ~Rotary();

    void setup();
    void loop();

    RotaryState GetState();
    uint8_t Counter() { return counter; }
    void ResetCounter() { counter = 0; }
};

#endif