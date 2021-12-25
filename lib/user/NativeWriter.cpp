// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifndef ARDUINO

#include "NativeWriter.h"

void NativeWriter::write(const char *heading, const char *label)
{
    printf("%s\n%s\n", heading, label);
    writeMenu();
}

void NativeWriter::writeMenu()
{
    printf("1-previous 2-next 3-select 4-exit-> ");
}

void NativeWriter::write(const char *heading, const char *label, int16_t value)
{
    printf("%s\n%s: %d\n", heading, label, value);
    writeMenu();
}
#endif
