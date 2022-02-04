// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifndef ARDUINO

#include "NativeWriter.h"

void NativeWriter::Write(const char *heading, const char *label)
{
    printf("%s\n%s\n", heading, label);
    writeMenu();
}

void NativeWriter::writeMenu()
{
    printf("1-previous 2-next 3-select 4-exit-> ");
}

void NativeWriter::Write(const char *heading, const char *label,
                         preset_base value, preset_base high)
{
    printf("%s\n%s: %u:%u\n", heading, label, value, high);
    writeMenu();
}
#endif
