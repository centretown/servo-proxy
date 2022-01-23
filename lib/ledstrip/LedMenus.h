// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"

#include "Menu.h"

#include "LedEndPoint.h"

extern const char *ledParmsText[];
extern const size_t ledParmsCount;
extern const char *ledSubText[];
extern const size_t ledSubLength;

void printFree();

class LedMenus
{
private:
    EndPoint *endPoint = NULL;

    static const char *ledParmsText[];
    static const size_t ledParmsCount;
    static const char *ledSubText[];
    static const size_t ledSubLength;

public:
    LedMenus(LedSegment *segments, size_t segCount);
    ~LedMenus();

    void addLedSubMenu(Menu *menu, size_t segCount);
    void initLedMenu(size_t segCount);
};
