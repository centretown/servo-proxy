// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifndef ARDUINO

#include "NativeStrips.h"

NativeStrips::NativeStrips(/* args */)
{
}

NativeStrips::~NativeStrips()
{
}

void NativeStrips::setPixelColor(uint16_t n, uint32_t c)
{
    printf("setColor:%u-%u\n", n, c);
}

void NativeStrips::show(uint16_t begin, uint16_t end)
{
    printf("show:%u-%u\n", begin, end);
}

#endif
