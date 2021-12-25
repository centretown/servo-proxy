// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifdef ARDUINO

#pragma once
#include <Arduino.h>
#include <RotaryEncoder.h>

typedef uint8_t RotaryState;

#define ROTARY_NOP 0
#define ROTARY_CLOCKWISE 0x01
#define ROTARY_COUNTER_CLOCKWISE 0x02
#define ROTARY_CLICK 0x04

class Rotary
{
private:
    RotaryEncoder &enc;
    uint8_t buttonPin = 44;

    RotaryState state = ROTARY_NOP;

    long lastPos = -999;
    int lastButtonState = LOW;
    uint64_t lastTime = 0;

    int32_t counter = 0;
    int32_t min = 0;
    int32_t max = 0;
    int32_t inc = 1;

public:
    Rotary(RotaryEncoder &enc, uint8_t b);
    ~Rotary();

    void setup();
    void tick();

    RotaryState GetState();
    int32_t Counter() { return counter; }
    int32_t Min() { return min; }
    int32_t Max() { return max; }

    void ResetCounter() { counter = 0; }
    void SetCounter(int32_t c) { counter = c; }
    void SetRange(int32_t hi, int32_t lo = 0);
};

#endif