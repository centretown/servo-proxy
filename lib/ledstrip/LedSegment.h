// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "LedPresets.h"
#include "LedWriter.h"

enum LedOperator : uint8_t
{
    STRIP_SOLID,       // solid light
    STRIP_BLINK,       // blink
    STRIP_WIPE,        // light sequentially
    STRIP_CYCLE,       // cycle thru colors
    STRIP_RAINBOW,     // visible spectrum across
    STRIP_CHASE,       // every 3rd light on and off
    STRIP_CHASE_CYCLE, // chase combined with cycle
    STRIP_CHASE_XMAS,  // chase red/green
    STRIP_RESET,       // reset to blank
    STRIP_NOP,         // do nothing promptly
    STRIP_OPERATOR_COUNT = STRIP_RESET + 1,
    STRIP_OUT_OF_BOUNDS = STRIP_NOP + 1, // too big
};

#define MAX_WAIT 0xffff
#define NULL_PRESET 0xff

typedef struct
{
    uint16_t count = 0;
    uint16_t max = 0;
} led_index;

class LedSegment;

class LedOperation
{
public:
    const char *Label;
    void (*setup)(LedSegment *cmd);
    void (*tick)(LedSegment *cmd);
    uint16_t presetFlags;
    inline bool HasPreset(uint16_t index)
    {
        return presetFlags & (1 << index);
    }
};

class LedSegment
{
private:
    uint16_t begin;
    uint16_t end;
    uint16_t pixelCount = 0;

    LedPresets presets;
    led_index counters[4];
    uint8_t theatreInc = 3;
    uint64_t next = 0L;
    LedOperator current = STRIP_NOP;
    LedWriter *ledWriter;

private:
    static LedOperation operations[];

    static inline bool isOperator(LedOperator op)
    {
        return (op < STRIP_OUT_OF_BOUNDS);
    }

    static void reset(LedSegment *s) { s->Reset(); }
    static void solidSetup(LedSegment *s) { s->solidSetup(); }
    static void solid(LedSegment *s) { s->solid(); }
    static void blinkSetup(LedSegment *s) { s->blinkSetup(); }
    static void blink(LedSegment *s) { s->blink(); }
    static void colorWipeSetup(LedSegment *s) { s->wipeSetup(); }
    static void colorWipe(LedSegment *s) { s->wipe(); }
    static void cycleSetup(LedSegment *s) { s->cycleSetup(); }
    static void cycle(LedSegment *s) { s->cycle(); }
    static void rainbowSetup(LedSegment *s) { s->rainbowSetup(); }
    static void rainbow(LedSegment *s) { s->rainbow(); }
    static void chaseSetup(LedSegment *s) { s->chaseSetup(); }
    static void chase(LedSegment *s) { s->chase(); }
    static void chaseCycleSetup(LedSegment *s) { s->chaseCycleSetup(); }
    static void chaseCycle(LedSegment *s) { s->chaseCycle(); }
    static void chaseXmasSetup(LedSegment *s) { s->chaseXmasSetup(); }
    static void chaseXmas(LedSegment *s) { s->chaseXmas(); }

public:
    inline static const char *Label(LedOperator op) { return operations[op].Label; }
    inline static uint8_t GetOperationCount() { return STRIP_OPERATOR_COUNT; }
    inline static LedOperation &GetOperation(LedOperator op) { return operations[op]; }
    static uint8_t GetPresetCount(LedOperator op);

public:
    LedSegment(uint16_t b, uint16_t e) : begin(b), end(e) { pixelCount = end - begin + 1; }
    ~LedSegment() {}
    void SetWriter(LedWriter *writer) { ledWriter = writer; }
    void Start(LedOperator op);
    void Start(LedOperator op, const unsigned *parms, size_t nparms);
    void Tick();
    void Reset();
    uint16_t Begin() { return begin; }
    uint16_t End() { return end; }
    Preset *GetPreset(LedOperator op, uint8_t idx);

private:
    static void nop(LedSegment *cmd) {}

    uint32_t wheel(uint8_t pos);
    void zeroCounters() { memset(counters, 0, sizeof(counters)); }
    void solidSetup();
    void solid();
    void blinkSetup();
    void blink();
    void wipeSetup();
    void wipe();
    void cycleSetup();
    void cycle();
    void rainbowSetup();
    void rainbow();
    void chaseSetup();
    void chase();
    void chaseCycleSetup();
    void chaseCycle();
    void chaseXmasSetup();
    void chaseXmas();
    bool nextStep();
};
