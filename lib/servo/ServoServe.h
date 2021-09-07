// Copyright (c) 2021 Dave Marsh. See LICENSE.

/*************************************************** 
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 8 servos, one after the other on the
  first 8 pins of the PCA9685

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815
  
  These drivers use I2C to communicate, 2 pins are required to  
  interface.

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <ServoEasing.h>

#define PAN 0
#define TILT 1

#define ERR_TOO_SHORT 1
#define ERR_NO_EQUAL 2
#define ERR_NOT_ENOUGH_ARGS 3
#define ERR_COMMAND_NOT_FOUND 4
#define ERR_OK 0

typedef struct
{
    uint8_t angle = 0;
    uint8_t speed = 45;
    uint8_t fresh = 0;
} servo_cmd_t;

class ServoServe
{
private:
    ServoEasing **servos;
    servo_cmd_t commands[16];
    const int8_t *expanderPins;
    const size_t count;
    void attach(ServoEasing *servo, int expanderPin);
    bool ready = false;

public:
    ServoServe(
        ServoEasing **servos,
        int8_t *expanderPins,
        size_t count) : servos(servos),
                        expanderPins(expanderPins),
                        count(count) {}

    ~ServoServe() {}

    void setup();
    void loop();
    void start();
    int process(const char *buf);
};
