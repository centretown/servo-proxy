// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifdef ARDUINO

#include "CameraListener.h"

bool CameraListener::ready()
{

    while (stream.available())
    {
        char ch = stream.read();
        if (bufferCount >= sizeof(buffer))
        {
            bufferCount = 0;
            snprintf(buffer, sizeof(buffer), "Overflow");
            messageReady = true;
            return true;
        }

        if (isprint(ch))
        {
            buffer[bufferCount] = ch;
            bufferCount++;
            break;
        }

        if (ch != 10)
        {
            break;
        }

        buffer[bufferCount] = 0;
        bufferCount = 0;
        if (buffer[0] == '?')
        {
            commandReady = true;
        }
        else
        {
            messageReady = true;
        }
        return true;
    }
    
    return false;
}

#endif
