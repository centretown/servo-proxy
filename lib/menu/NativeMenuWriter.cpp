// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifndef ARDUINO

#include "NativeMenuWriter.h"

void NativeMenuWriter::write(const char *heading, const char *label,
                             int32_t *value)
{
    if (value != NULL)
    {
        printf("%s\n%s: %d\n", heading, label, *value);
    }
    else
    {
        printf("%s\n%s\n", heading, label);
    }
    printf("1-previous 2-next 3-select 4-exit-> ");
}

#endif
