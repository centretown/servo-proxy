// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include <Arduino.h>

#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "ServoServe.h"

SoftwareSerial camSerial(2, 3); // RX, TX

// use different address
// Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// use different address and I2C interface
const uint8_t pwmAddress = 0x40;
// Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(pwmAddress, Wire);

// use the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// pwm, number of servos
ServoServe servoServe(pwm, pwmAddress, 2);

void setup()
{
  Serial.begin(38400);
  // Serial.println("8 channel Servo test!");
  // sync data rate with peer
  camSerial.begin(38400);

  servoServe.setup();
  // Serial.println("servo setup");
  servoServe.start();
  // Serial.println("servo started");
}

void loop()
{
  // servoServe.loopServo();
  if (camSerial.available())
    Serial.write(camSerial.read());
  if (Serial.available()) {
    camSerial.write(Serial.read());
  }
  delay(5);
}