// Copyright (c) 2021 Dave Marsh. See LICENSE.
#if defined(USE_OLED_LIB)

#pragma once
#include "base.h"
#include "ResponseWriter.h"
#include "OledDisplay.h"

class OledWriter : public ResponseWriter
{
private:
    OledDisplay &oled;

public:
    OledWriter(OledDisplay &oled) : oled(oled) {}
    ~OledWriter() {}

    virtual void Write(const char *heading, const char *label)
    {
        oled.drawMenu(heading, label);
    }

    virtual void Write(const char *heading, const char *label,
                       preset_base value, preset_base high)
    {
        oled.drawMenu(heading, label, value, high);
    }
};

#endif // USE_OLED_LIB