// Copyright (c) 2022 Dave Marsh. See LICENSE.

#include "base.h"

char *PresetBaseFormat(char *buffer, size_t bufLen, preset_base v)
{
    snprintf(buffer, bufLen, PRESET_FORMAT, v);
    return buffer;
}
