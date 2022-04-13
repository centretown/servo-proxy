// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "Preset.h"

class ResponseWriter
{
public:
    ResponseWriter() {}
    virtual ~ResponseWriter() {}

    virtual void Write(const char *heading, const char *label) = 0;
    virtual void Write(const char *heading, const char *label,
                       preset_base value, preset_base high, bool edit = false) = 0;
};
