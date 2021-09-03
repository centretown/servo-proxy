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

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN 150  // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600  // This is the 'maximum' pulse length count (out of 4096)
#define USMIN 600     // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX 2400    // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

class ServoServe
{
private:
    Adafruit_PWMServoDriver &pwm;
    const uint8_t pwmAddress;
    uint8_t servoCount;
    // our servo # counter
    uint8_t servoNum = 0;

public:
    ServoServe(Adafruit_PWMServoDriver &pwm,
               uint8_t pwmAddress, 
               uint8_t servoCount) : pwm(pwm), pwmAddress(pwmAddress), servoCount(servoCount) {}
    ~ServoServe() {}

    int setup();
    int start();
    void setServoPulse(uint8_t n, double pulse);
    void loopServo(int count = 2);
    void microTest(int count);
    void pulseTest(int count);
    void wave(int count);
};
