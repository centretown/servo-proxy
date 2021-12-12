// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifndef ARDUINO

#include "NativeMenuReader.h"

MenuEvent NativeMenuReader::GetEvent()
{
    unsigned option;
    int count = scanf("%u", &option);
    if (count > 0)
    {
        switch (option)
        {
        case 1:
            return MENU_PREVIOUS;
        case 2:
            return MENU_NEXT;
        case 3:
            return MENU_SELECT;
        }
    }
    return MENU_NOP;
}

#endif