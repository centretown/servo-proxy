// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifndef ARDUINO

#include "Menu.h"
#include "LedMenus.h"
#include "NativeReader.h"
#include "NativeWriter.h"
#include "LedSegment.h"

#define USE_SERVO_LIB
#define USE_LEDSTRIP_LIB

#include <time.h>
uint64_t millis()
{
    static uint64_t currentMillis = 0;
    currentMillis += 299;
    return currentMillis;
}

void printFree() {}

LedSegment segs[] = {
    LedSegment(0, 9),
    LedSegment(10, 19),
    LedSegment(20, 29),
    LedSegment(30, 39),
};

size_t ledSegmentCount = 4;

LedMenus ledMenus(segs, ledSegmentCount);
NativeReader nativeReader;
NativeWriter nativeWriter;

void displayPresets()
{
    LedSegment *seg = &segs[0];
    for (auto j = 0; j < STRIP_OPERATOR_COUNT; j++)
    {
        LedOperator op = (LedOperator)j;
        auto count = seg->GetPresetCount(op);
        printf("%s %u\n", LedSegment::Label(op), count);
        for (auto k = 0; k < count; k++)
        {
            Preset *preset = seg->GetPreset(op, k);
            printf("\t%s:%u\n", preset->Label(), preset->Get());
        }
    }
}

void testMenu(Menu *menu)
{
    static uint16_t depth = 0;
    depth++;
    Menu *m = menu;
    printf("%*s %s %u\n", depth, " ", m->Label(), m->Count());
    // return;
    for (uint8_t i = 0; i < m->Count(); i++)
    {
        testMenu(m->Node(i));
    }
    depth--;
}

#include "LedPresets.h"
void testPreset()
{
    char buffer[32];
    LedPresets presets;
    for (uint8_t i = 0; i < STRIP_PRESET_COUNT; i++)
    {
        LedPropertyID op = LedPropertyID(i);
        Preset *p = presets.GetPreset(op);
        printf("%s:%s\n", p->Label(),
               p->Format(buffer, sizeof(buffer)));
    }

    Preset *timer = presets.GetPreset(STRIP_PRESET_TIMER);
    Preset *repeat = presets.GetPreset(STRIP_PRESET_REPEAT);
    Preset *pulse = presets.GetPreset(STRIP_PRESET_PULSE);
    pulse->Set(100);
    printf("%s:%s\n", pulse->Label(),
           pulse->Format(buffer, sizeof(buffer)));
    for (preset_base i = 1; i < 999; i++)
    {
        timer->Set(TIMER_PRESET_MINUTE * i);
        printf("%s:%s\n", pulse->Label(),
               pulse->Format(buffer, sizeof(buffer)));
        printf("%s:%s\n", timer->Label(),
               timer->Format(buffer, sizeof(buffer)));
        printf("%s:%s\n", repeat->Label(),
               repeat->Format(buffer, sizeof(buffer)));
    }

    Preset *dimmer = presets.GetPreset(STRIP_PRESET_DIMMER);
    Preset *red = presets.GetPreset(STRIP_PRESET_RED);
    Preset *green = presets.GetPreset(STRIP_PRESET_GREEN);
    Preset *blue = presets.GetPreset(STRIP_PRESET_BLUE);
    red->Set(255);
    green->Set(127);
    blue->Set(63);
    for (preset_base i = 1; i <= 10; i++)
    {
        dimmer->Set(10 * i);
        printf("bright:%u rgb:%u-%u-%u\n", presets.Dimmer(),
               presets.Red(), presets.Green(), presets.Blue());
    }
}

int main(int argc, char **argv)
{
    ledMenus.Build("LED Strips");
    Menu::SetReader(&nativeReader);
    Menu::SetWriter(&nativeWriter);
    displayPresets();
    // return 0;
    testMenu(&rootMenu);
    testPreset();

    Menu::setup();
    while (true)
    {
        Menu::loop();
    }
    return 0;
}

#endif