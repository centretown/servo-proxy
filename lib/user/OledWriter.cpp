// Copyright (c) 2021 Dave Marsh. See LICENSE.
#if defined(USE_OLED_LIB)

#include "OledWriter.h"

void OledWriter::writeMenu(const char *heading, const char *label,
                           int32_t *value)
{
    oled.drawMenu(heading, label, value);
}

#endif // USE_OLED_LIB
