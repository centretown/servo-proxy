// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "ColorPreset.h"

preset_base ColorPreset::Filtered()
{
    preset_base v = value;
    if (filter == NULL)
    {
        return v;
    }
    v *= filter->Get();
    v /= filter->High();
    return v;
}
