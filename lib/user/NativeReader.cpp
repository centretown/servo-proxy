// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifndef ARDUINO

#include "NativeReader.h"

UserEvent NativeReader::GetEvent()
{
    unsigned option;
    int count = scanf("%u", &option);
    if (count > 0)
    {
        switch (option)
        {
        case 1:
            return USER_PREVIOUS;
        case 2:
            return USER_NEXT;
        case 3:
            return USER_SELECT;
        }
    }
    return USER_WAIT;
}

#endif