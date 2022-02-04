// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#ifdef ARDUINO
#include <Arduino.h>
#define PRESET_FORMAT "%lu"
#else
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
uint64_t millis();
#define PRESET_FORMAT "%u"
#endif

typedef uint32_t preset_base;
char *PresetBaseFormat(char *buffer, size_t bufLen, preset_base v);
