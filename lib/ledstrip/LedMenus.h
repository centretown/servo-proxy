// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"

#include "Menu.h"

#include "LedSegment.h"
#include "LedPresets.h"
#include "LedEndPoint.h"

void printFree();

class LedMenus
{
private:
    LedSegment *segments;
    size_t segCount = 0;
    EndPoint *endPoint = NULL;

public:
    LedMenus(LedSegment *segments, size_t segCount);
    ~LedMenus();

    void Build(const char *label);

private:
    void addMenus(Menu *menu);
};
