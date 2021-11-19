// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include <Arduino.h>
#include <MicroTerm.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include "CameraListener.h"

#include "ServoServe.h"
#include "LedStrip.h"

// for ESP32 LED_BUILTIN is defined as static const uint8_t LED_BUILTIN = 2;
#if !defined(LED_BUILTIN) && !defined(ESP32)
#define LED_BUILTIN PB1
#endif
// On the Zero and others we switch explicitly to SerialUSB
#if defined(ARDUINO_ARCH_SAMD)
#define Serial SerialUSB
#endif

// #define LEDSTRIP_PIN16 11
#define LEDSTRIP_PIN23 11
#define LEDSTRIP_PIN4 12
#define LEDSTRIP_PIN8 13

// only interrupt change pins on nano
// #define SOFT_RX 2
// #define SOFT_TX 3
// SoftwareSerial Serial1(SOFT_RX, SOFT_TX); // RX, TX
/*
 * Constructor to specify the expander address (needed at least if you use more than one expander board)
 * and to specify the I2C implementation library.
 * This can be done for each servo separately, but you can not (yet) mix the 3 different
 * Servo implementation libraries (Arduino Servo, Lightweight Servo and I2C Expansion Board)
 */
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
ServoEasing Servo5;
ServoEasing Servo6;
ServoEasing Servo7;
ServoEasing Servo8;
ServoEasing Servo9;

#endif

ServoEasing *servos[] = {&Servo1, &Servo2, &Servo3,
                         &Servo4, &Servo5, &Servo6,
                         &Servo7, &Servo8, &Servo9};
ServoEasing **pServos = servos;

// Only works on pin 2, 3, 5, 6, 7, 8, 44, 45 and 46 on Arduino Mega!
int8_t expanderPins[] = {2, 3, 5, 6, 7, 8, 44, 45, 46};
ServoServe servoServe(pServos, expanderPins, sizeof(expanderPins) / sizeof(expanderPins)[0]);
char usrBuffer[81] = {0};
char testBuffer[81] = {0};
MicroTerm usrTerm(Serial, usrBuffer, sizeof(usrBuffer));
CameraListener camTerm(Serial1);

Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(4, LEDSTRIP_PIN4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip8 = Adafruit_NeoPixel(8, LEDSTRIP_PIN8, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip23 = Adafruit_NeoPixel(23, LEDSTRIP_PIN23, NEO_BRG + NEO_KHZ800);
// Adafruit_NeoPixel strip16 = Adafruit_NeoPixel(7, LEDSTRIP_PIN16, NEO_BRG + NEO_KHZ800);
LedSegment segs[] = {
    LedSegment(0, 34),  //0
    LedSegment(0, 3),   //1
    LedSegment(4, 11),  //2
    LedSegment(12, 34), //3
    LedSegment(15, 34), //4
    LedSegment(0, 0),   //5
    LedSegment(0, 1),   //6
    LedSegment(4, 4),   //7
    LedSegment(4, 4),   //8
    LedSegment(34, 34), //9
    LedSegment(30, 33), //10
};

Adafruit_NeoPixel *strips[] = {
    &strip4,
    &strip8,
    &strip23,
    // &strip16,
};

LedStrip led = LedStrip(strips, sizeof(strips) / sizeof(strips[0]),
                        segs, sizeof(segs) / sizeof(segs[0]));

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
#if defined(__AVR_ATmega32U4__)
  // delay for Leonardo, this loops forever on Maple Serial
  while (!Serial)
    ;
#endif
#if defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL)
  // ensure everything displayed
  delay(2000);
#endif
  // Identify
  Serial.println(F("START " __FILE__ " from " __DATE__));

  led.setup();
  Serial.print("led strip setup: ");
  Serial.print(led.numPixels());
  Serial.println(" pixels defined.");

  servoServe.setup();
  Serial.println("servo setup");
  usrTerm.setup(".usr");

  servoServe.start();
  Serial1.flush();
}

void processError(int err);

void loop()
{
  servoServe.loop();
  led.loop();

  if (usrTerm.ready())
  {
    const char *buf = usrTerm.get();
    if (buf != NULL)
    {
      char c = buf[0];
      if (c == '?')
      {
        strncpy(testBuffer, buf, sizeof(testBuffer));
        usrTerm.print(F("servo: "));
        usrTerm.println(testBuffer);
        int err = servoServe.process(testBuffer);
        processError(err);
      }
      else if (c == '!')
      {
        strncpy(testBuffer, buf, sizeof(testBuffer));
        usrTerm.print(F("led: "));
        usrTerm.println(testBuffer);
        int err = led.process(testBuffer);
        processError(err);
      }
      else
      {
        Serial1.println(buf);
      }
    }
  }

  if (camTerm.ready())
  {
    if (camTerm.hasMessage())
    {
      usrTerm.println(camTerm.get());
      return;
    }

    char *camBuffer = camTerm.get();
    int err = servoServe.process(camBuffer);
    processError(err);
    if (err > 0)
    {
      return;
    }
    usrTerm.println(camBuffer);
    // usrTerm.prompt();
    return;
  }
}

void processError(int err)
{
  switch (err)
  {
  case ERR_SERVO_NOT_ENOUGH_ARGS:
    usrTerm.println(F("ERR_NOT_ENOUGH_ARGS"));
    return;
  case ERR_SERVO_NOT_FOUND:
    usrTerm.println(F("ERR_NOT_FOUND"));
    return;
  case ERR_SERVO_INDEX:
    usrTerm.println(F("ERR_INDEX"));
    return;
  case ERR_SERVO_BAD_VALUE:
    usrTerm.println(F("ERR_BAD_VALUE"));
    return;
  }
}