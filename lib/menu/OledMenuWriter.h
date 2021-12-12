// Copyright (c) 2021 Dave Marsh. See LICENSE.
#if defined(USE_OLED_LIB)

#pragma once
#include "base.h"
#include "MenuWriter.h"
#include "OledDisplay.h"

class OledMenuWriter : public MenuWriter
{
private:
    OledDisplay &oled;

public:
    OledMenuWriter(OledDisplay &oled) : oled(oled) {}
    ~OledMenuWriter() {}

    virtual void write(const char *heading, const char *label,
                           int32_t *value = NULL)
    {
        oled.drawMenu(heading, label, value);
    }
};

#endif