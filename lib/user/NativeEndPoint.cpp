// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifndef ARDUINO
#include "NativeEndPoint.h"

void NativeEndPoint::start()
{
    printf("i=%u c=%u p=%u n=%d\n",
           index, command, parameter, counter);
}
#endif