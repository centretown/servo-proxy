// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#define USE_SERVO_LIB

#define USE_LEDSTRIP_LIB

#define USE_OLED_LIB

#define USE_TOUCH_LIB

//////////////////////////////
#if defined(USE_SERVO_LIB)
#include "ServoServe.h"
#if defined(USE_PCA9685_SERVO_EXPANDER)
#if defined(ARDUINO_SAM_DUE)
ServoEasing Servo1(PCA9685_DEFAULT_ADDRESS, &Wire1);
ServoEasing Servo2(PCA9685_DEFAULT_ADDRESS, &Wire1);
ServoEasing Servo3(PCA9685_DEFAULT_ADDRESS, &Wire1);
ServoEasing Servo4(PCA9685_DEFAULT_ADDRESS, &Wire1);
#else
ServoEasing Servo1(PCA9685_DEFAULT_ADDRESS, &Wire);
ServoEasing Servo2(PCA9685_DEFAULT_ADDRESS, &Wire);
ServoEasing Servo3(PCA9685_DEFAULT_ADDRESS, &Wire);
ServoEasing Servo4(PCA9685_DEFAULT_ADDRESS, &Wire);
#endif
#else
ServoEasing Servo1;
ServoEasing Servo2;
ServoEasing Servo3;
ServoEasing Servo4;
#endif

ServoEasing *servos[] = {&Servo1, &Servo2, &Servo3, &Servo4};
ServoEasing **pServos = servos;
// Only works on pin 2, 3, 5, 6, 7, 8, 44, 45 and 46 on Arduino Mega!
int8_t expanderPins[] = {2, 3, 5, 6};
ServoServe servoServe(pServos, expanderPins,
                      sizeof(expanderPins) / sizeof(expanderPins)[0]);

#endif
//////////////////////////////

//////////////////////////////
#if defined(USE_OLED_LIB)
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
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
OledDisplay oled(display, SCREEN_ADDRESS);
#endif
//////////////////////////////

//////////////////////////////
#if defined(USE_LEDSTRIP_LIB)
#include "LedStrips.h"

#ifdef __AVR__
#include <avr/power.h>
#endif

#define LEDSTRIP_PIN11 11
#define LEDSTRIP_PIN12 12
#define LEDSTRIP_PIN13 13

Adafruit_NeoPixel strip19(19, LEDSTRIP_PIN11, NEO_BRG + NEO_KHZ800);
Adafruit_NeoPixel strip4(4, LEDSTRIP_PIN12, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip8(8, LEDSTRIP_PIN13, NEO_GRB + NEO_KHZ800);

// Adafruit_NeoPixel strip16 = Adafruit_NeoPixel(7, LEDSTRIP_PIN16, NEO_BRG + NEO_KHZ800);
LedSegment segs[] = {
    LedSegment(0, 30),  //0
    LedSegment(0, 18),  //1
    LedSegment(19, 22), //2
    LedSegment(23, 30), //3
};

Adafruit_NeoPixel *strips[] = {
    &strip19,
    &strip4,
    &strip8,
};

LedStrips led(strips, sizeof(strips) / sizeof(strips[0]),
                        segs, sizeof(segs) / sizeof(segs[0]));

#endif
//////////////////////////////

//////////////////////////////
#if defined(USE_TOUCH_LIB)
#include "TouchSensor.h"

#define SENSOR_PIN 44
TouchSensor touch(SENSOR_PIN);

#endif
//////////////////////////////
