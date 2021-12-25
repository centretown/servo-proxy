// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#if defined(ARDUINO)

#if defined(USE_SERVO_LIB)
#include "ServoServe.h"

extern ServoEasing Servo1;
extern ServoEasing Servo2;
extern ServoEasing Servo3;
extern ServoEasing Servo4;

extern ServoEasing *servos[];
extern ServoEasing **pServos;
// Only works on pin 2, 3, 5, 6, 7, 8, 44, 45 and 46 on Arduino Mega!
extern int8_t expanderPins[];
extern size_t expanderPinCount;
extern ServoServe servoServe;
#endif // USE_SERVO_LIB

#ifdef USE_OLED_LIB
#include "OledDisplay.h"

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// UNO:       A4(SDA), A5(SCL)
// MEGA 2560: 20(SDA), 21(SCL)
// LEONARDO:   2(SDA),  3(SCL), ...
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 32    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // 0x3D for 128x64, 0x3C for 128x32
extern Adafruit_SSD1306 display;
extern OledDisplay oled;
#endif // USE_OLED_LIB

#ifdef USE_LEDSTRIP_LIB
#include "LedStrips.h"
#include "LedEndPoint.h"

#ifdef __AVR__
#include <avr/power.h>
#endif

#define LEDSTRIP_PIN10 10
#define LEDSTRIP_PIN11 11
#define LEDSTRIP_PIN12 12
#define LEDSTRIP_PIN13 13

extern Adafruit_NeoPixel strip19;
extern Adafruit_NeoPixel strip15;
extern Adafruit_NeoPixel strip4;
extern Adafruit_NeoPixel strip8;
extern LedSegment segs[];
extern size_t ledSegmentCount;

extern Adafruit_NeoPixel *strips[];
extern size_t ledStripCount;

extern LedStrips led;
extern LedEndPoint ledPoint;

#endif // USE_LEDSTRIP_LIB

#ifdef USE_TOUCH_LIB
#include "TouchSensor.h"
#define TOUCH_SENSOR_PIN 44
extern TouchSensor touch;
#endif // USE_TOUCH_LIB

#if defined(USE_ROTARY_LIB)
#include <Rotary.h>
#define ROTARY_A 15
#define ROTARY_B 14
#define ROTARY_BUTTON 34
extern RotaryEncoder enc;
extern Rotary rotary;
#endif // USE_ROTARY_LIB

#endif // ARDUINO
