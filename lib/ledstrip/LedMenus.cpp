// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "LedMenus.h"

LedMenus::LedMenus(LedSegment *segments, size_t segCount) : segments(segments), segCount(segCount)
{
    endPoint = new LedEndPoint(segments, segCount);
}

LedMenus::~LedMenus()
{
    free(endPoint);
}

void LedMenus::Build(const char *label)
{
    printFree();
    Menu *ledMenu = rootMenu.Add(
        new Menu(label, 2, ENDPOINT_NAV, 1, endPoint));
    addMenus(ledMenu);
    ledMenu->AddExit();
    printFree();
}

void LedMenus::addMenus(Menu *menu)
{
    LedSegment *seg = &segments[0];
    uint8_t opCount = LedSegment::GetOperationCount();
    Menu *subMenu = menu->Add(
        new Menu("Strip", opCount + 1, ENDPOINT_INDEX, segCount));
    for (uint8_t i = 0; i < opCount; i++)
    {
        LedOperator op = (LedOperator)i;
        uint8_t presetCount = LedSegment::GetPresetCount(op);
        Menu *opMenu = new Menu(LedSegment::Label(op), presetCount + 1,
                                ENDPOINT_RUN, 1);
        subMenu->Add(opMenu);
        for (uint8_t j = 0; j < presetCount; j++)
        {
            Preset *preset = seg->GetPreset(op, j);
            Menu *item = new Menu(preset->Label(), 1, ENDPOINT_EDIT);
            opMenu->Add(item);
        }
        opMenu->AddExit();
    }
    subMenu->AddExit();
}