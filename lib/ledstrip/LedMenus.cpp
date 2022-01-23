// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "LedMenus.h"

const char *LedMenus::ledParmsText[] = {"Red", "Green", "Blue", "Repeat", "Delay"};
const size_t LedMenus::ledParmsCount = sizeof(ledParmsText) / sizeof(ledParmsText[0]);
const char *LedMenus::ledSubText[] = {"Solid", "Blink", "Wipe", "Cycle",
                                      "Rainbow", "Chase", "CycleChase", "Xmas", "Reset"};
const size_t LedMenus::ledSubLength = sizeof(ledSubText) / sizeof(ledSubText[0]);

LedMenus::LedMenus(LedSegment *segments, size_t segCount)
{
    endPoint = new LedEndPoint(segments, segCount);
}

LedMenus::~LedMenus()
{
    free(endPoint);
}

void LedMenus::addLedSubMenu(Menu *menu, size_t segCount)
{
    // size_t segCount = sizeof(segs) / sizeof(segs[0]);
    // add 1 for setup, 1 for exit
    Menu *sub = menu->Add(new Menu("Strip", ledSubLength + 2, segCount));
    addEndpoints(sub, ledSubText, ledSubLength,
                 ledParmsText, ledParmsCount,
                 endPoint, endPoint);
    addExit(sub);
}

void LedMenus::initLedMenu(size_t segCount)
{
    // add item for exit
    Menu *ledMenu = rootMenu.Add(new Menu("LED Strips", 2));
    addLedSubMenu(ledMenu, segCount);
    printFree();
    addExit(ledMenu);
    printFree();
}
