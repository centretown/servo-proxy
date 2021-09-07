// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <MicroTerm.h>
#include "ServoServe.h"

// for ESP32 LED_BUILTIN is defined as static const uint8_t LED_BUILTIN = 2;
#if !defined(LED_BUILTIN) && !defined(ESP32)
#define LED_BUILTIN PB1
#endif
// On the Zero and others we switch explicitly to SerialUSB
#if defined(ARDUINO_ARCH_SAMD)
#define Serial SerialUSB
#endif

// only interrupt change pins on nano
#define SOFT_RX 2
#define SOFT_TX 3
SoftwareSerial camSerial(SOFT_RX, SOFT_TX); // RX, TX
/*
 * Constructor to specify the expander address (needed at least if you use more than one expander board)
 * and to specify the I2C implementation library.
 * This can be done for each servo separately, but you can not (yet) mix the 3 different
 * Servo implementation libraries (Arduino Servo, Lightweight Servo and I2C Expansion Board)
 */
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

ServoEasing *servos[] = {&Servo1, &Servo2, &Servo3, &Servo4};
ServoEasing **pServos = servos;
int8_t expanderPins[] = {0, 1};
ServoServe servoServe(pServos, expanderPins, sizeof(expanderPins) / sizeof(expanderPins)[0]);
char usrBuffer[81] = {0};
MicroTerm usrTerm(Serial, usrBuffer, sizeof(usrBuffer));

void setup()
{
  camSerial.begin(38400);
  Serial.begin(38400);
#if defined(__AVR_ATmega32U4__)
  while (!Serial)
    ; //delay for Leonardo, but this loops forever for Maple Serial
#endif
#if defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL)
  delay(2000); // To be able to connect Serial monitor after reset and before first printout
#endif
  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_SERVO_EASING));

  servoServe.setup();
  Serial.println("servo setup");
  // sync data rate with peer
  usrTerm.setup(".usr");
  // camTerm.setup(".cam");
  // camTerm.user(false); //no prompt or echo
  servoServe.start();
  servoServe.process("?pan=90");
  servoServe.process("?tilt=90");
  camSerial.flush();
}

char camBuffer[32];
size_t camCount = 0;

void getCameraInput();

void loop()
{
  servoServe.loop();

  if (usrTerm.ready())
  {
    const char *buf = usrTerm.get();
    if (buf != NULL)
    {
      camSerial.println(buf);
    }
  }

  getCameraInput();
}

void getCameraInput()
{
  while (camSerial.available())
  {
    char ch = camSerial.read();
    if (camCount >= sizeof(camBuffer))
    {
      camCount = 0;
      usrTerm.println("Overflow");
      return;
    }

    if (ch != 10 && isprint(ch))
    {
      camBuffer[camCount] = ch;
      camCount++;
      return;
    }

    camBuffer[camCount] = 0;
    camCount = 0;

    switch (servoServe.process(camBuffer))
    {
    case ERR_TOO_SHORT:
      Serial.println(F("ERR_TOO_SHORT"));
      return;
    case ERR_NO_EQUAL:
      Serial.println(F("ERR_NO_EQUAL"));
      return;
    case ERR_NOT_ENOUGH_ARGS:
      Serial.println(F("ERR_NOT_ENOUGH_ARGS"));
      return;
    case ERR_COMMAND_NOT_FOUND:
      Serial.println(F("ERR_COMMAND_NOT_FOUND"));
      return;
    }
    Serial.println(camBuffer);
    return;
  }
}