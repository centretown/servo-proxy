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
#include "OledDisplay.h"
#include "TouchSensor.h"

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

#define LEDSTRIP_PIN9a 7
#define LEDSTRIP_PIN9b 8
#define LEDSTRIP_PIN9c 9
#define LEDSTRIP_PIN9d 10

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 32    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
OledDisplay oled(display, SCREEN_ADDRESS);
#define SENSOR_PIN 44
TouchSensor touch(SENSOR_PIN);

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
// ServoEasing Servo5;
// ServoEasing Servo6;
// ServoEasing Servo7;
// ServoEasing Servo8;
// ServoEasing Servo9;

#endif

// ServoEasing *servos[] = {&Servo1, &Servo2, &Servo3,
//                          &Servo4, &Servo5, &Servo6,
//                          &Servo7, &Servo8, &Servo9};
ServoEasing *servos[] = {&Servo1, &Servo2, &Servo3, &Servo4};
ServoEasing **pServos = servos;

// Only works on pin 2, 3, 5, 6, 7, 8, 44, 45 and 46 on Arduino Mega!
int8_t expanderPins[] = {2, 3, 5, 6};
ServoServe servoServe(pServos, expanderPins, sizeof(expanderPins) / sizeof(expanderPins)[0]);
char usrBuffer[81] = {0};
char testBuffer[81] = {0};
MicroTerm usrTerm(Serial, usrBuffer, sizeof(usrBuffer));
CameraListener camTerm(Serial1);

Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(4, LEDSTRIP_PIN4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip8 = Adafruit_NeoPixel(8, LEDSTRIP_PIN8, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip23 = Adafruit_NeoPixel(23, LEDSTRIP_PIN23, NEO_BRG + NEO_KHZ800);

Adafruit_NeoPixel strip9a = Adafruit_NeoPixel(9, LEDSTRIP_PIN9a, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip9b = Adafruit_NeoPixel(9, LEDSTRIP_PIN9b, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip9c = Adafruit_NeoPixel(9, LEDSTRIP_PIN9c, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip9d = Adafruit_NeoPixel(9, LEDSTRIP_PIN9d, NEO_GRB + NEO_KHZ800);

// Adafruit_NeoPixel strip16 = Adafruit_NeoPixel(7, LEDSTRIP_PIN16, NEO_BRG + NEO_KHZ800);
LedSegment segs[] = {
    LedSegment(0, 70),  //0
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
    LedSegment(35, 70), //11
    LedSegment(35, 43), //12
    LedSegment(44, 52), //13
    LedSegment(53, 60), //14
    LedSegment(61, 70), //15

};

Adafruit_NeoPixel *strips[] = {
    &strip4,
    &strip8,
    &strip23,
    &strip9a,
    &strip9b,
    &strip9c,
    &strip9d,
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

  oled.setup();
  if (oled.isActive())
  {
    Serial.println("OLED OK!");
  }
  else
  {
    Serial.println("OLED FAILED!");
  }

  touch.setup();
  Serial.println("Touch Sensor OK!");
}

void processError(int err);
uint8_t pic = 0;
bool blinking = false;

void loop()
{
  touch.loop();
  ActionState state = touch.getState();
  if (state == TOUCH_TAP)
  {
    pic++;
    if (pic > 2)
    {
      pic = 0;
    }
    switch (pic)
    {
    case 0:
      oled.dartboard();
      break;
    case 1:
      oled.dog();
      break;
    case 2:
      oled.panda();
      break;
    }
  }
  else if (state == TOUCH_HOLD)
  {
    unsigned parms[1] = {0};
    if (blinking)
    {
      segs[0].start(1, parms, 1);
      blinking = false;
    }
    else
    {
      segs[0].start(8, parms, 1);
      blinking = true;
    }
  }

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
      else if (c == '$')
      {
        strncpy(testBuffer, buf, sizeof(testBuffer));
        usrTerm.print(F("oled: "));
        usrTerm.println(testBuffer);
        int err = oled.process(testBuffer);
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