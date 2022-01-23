// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "LedPresets.h"
#include "LedWriter.h"

enum LedOperator
{
    STRIP_SOLID,         // solid light
    STRIP_BLINK,         // blink
    STRIP_WIPE,          // light sequentially
    STRIP_CYCLE,         // cycle thru colors
    STRIP_RAINBOW,       // visible spectrum across
    STRIP_CHASE,         // every 3rd light on and off
    STRIP_CHASE_CYCLE,   // chase combined with cycle
    STRIP_CHASE_XMAS,    // chase combined with xmas
    STRIP_RESET,         // reset to blank
    STRIP_NOP,           // do nothing quickly
    STRIP_OUT_OF_BOUNDS, // too big
    STRIP_FIRST = STRIP_SOLID,
    STRIP_LAST = STRIP_CHASE_XMAS
};

#define MAX_WAIT 0xffff
#define NULL_PRESET 0xff

typedef struct
{
    uint16_t count = 0;
    uint16_t max = 0;
} led_index;

class LedSegment
{
private:
    uint16_t begin;
    uint16_t end;

    LedPresets presets;
    led_index counters[4];
    uint8_t theatreInc = 3;
    uint64_t next = 0L;
    LedOperator current = STRIP_NOP;

public:
    static void Reset(LedSegment *s) { s->Reset(); }
    static void SolidSetup(LedSegment *s) { s->solidSetup(); }
    static void Solid(LedSegment *s) { s->solid(); }
    static void BlinkSetup(LedSegment *s) { s->blinkSetup(); }
    static void Blink(LedSegment *s) { s->blink(); }
    static void ColorWipeSetup(LedSegment *s) { s->wipeSetup(); }
    static void ColorWipe(LedSegment *s) { s->wipe(); }
    static void CycleSetup(LedSegment *s) { s->cycleSetup(); }
    static void Cycle(LedSegment *s) { s->cycle(); }
    static void RainbowSetup(LedSegment *s) { s->rainbowSetup(); }
    static void Rainbow(LedSegment *s) { s->rainbow(); }
    static void ChaseSetup(LedSegment *s) { s->chaseSetup(); }
    static void Chase(LedSegment *s) { s->chase(); }
    static void ChaseCycleSetup(LedSegment *s) { s->chaseCycleSetup(); }
    static void ChaseCycle(LedSegment *s) { s->chaseCycle(); }
    static void ChaseXmasSetup(LedSegment *s) { s->chaseXmasSetup(); }
    static void ChaseXmas(LedSegment *s) { s->chaseXmas(); }

public:
    LedSegment(uint16_t b, uint16_t e) : begin(b), end(e) {}
    ~LedSegment() {}
    void SetWriter(LedWriter *writer) { ledWriter = writer; }
    void Start(LedOperator op);
    void Start(LedOperator op, const unsigned *parms, size_t nparms);
    void Tick();
    void Reset();
    uint16_t Begin() { return begin; }
    uint16_t End() { return end; }
    LedPresets &Presets() { return presets; }

private:
    LedWriter *ledWriter;

    class Entry
    {
    public:
        const char *Label;
        void (*setup)(LedSegment *cmd);
        void (*tick)(LedSegment *cmd);
        uint16_t presetFlags;
        inline bool HasPreset(uint8_t index)
        {
            return (1 << index) & presetFlags;
        }
    };

    static Entry entry_table[];

    static inline bool isOperator(LedOperator op)
    {
        return (op < STRIP_OUT_OF_BOUNDS);
    }

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
    inline uint16_t count() { return end - begin + 1; }
    bool nextStep();
};
