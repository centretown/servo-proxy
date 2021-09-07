// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "Console.h"

void Console::parse(const char *buf)
{
    size_t len = strlen(buf);
    if (len < 9)
    {
        return;
    }
    if (buf[0] != '?')
    {
        return;
    }

    char buffer[16] = {0};
    int angle = 0;
    int nitems = sscanf("?%15=%d", buffer, &angle);
    if (nitems != 2)
    {
        return;
    }

    if (!strcmp("pan", buffer))
    {
        cmd.id = PAN;
    }
    else if (!strcmp("tilt", buffer))
    {
        cmd.id = TILT;
    }
    else
    {
        return;
    }

    cmd.angle = angle;
    cmd.speed = 45;
    cmd_ready = true;
}

bool Console::ready()
{
    return cmd_ready;
}
