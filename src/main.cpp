// Copyright (c) 2021 Dave Marsh. See LICENSE.
#ifdef ARDUINO

#define USE_SERVO_LIB
#define USE_LEDSTRIP_LIB
#define USE_OLED_LIB
#define USE_ROTARY_LIB
// #define USE_TOUCH_LIB

#include <Arduino.h>
#include <MicroTerm.h>
#include "CameraListener.h"
#include "shared.h"
#include "config.h"

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
#endif // ARDUINO_SAM_DUE

#else
ServoEasing Servo1;
ServoEasing Servo2;
ServoEasing Servo3;
ServoEasing Servo4;
#endif // USE_PCA9685_SERVO_EXPANDER

ServoEasing *servos[] = {&Servo1, &Servo2, &Servo3, &Servo4};
ServoEasing **pServos = servos;
// Only works on pin 2, 3, 5, 6, 7, 8, 44, 45 and 46 on Arduino Mega!
int8_t expanderPins[] = {2, 3, 5, 6};
size_t expanderPinCount = sizeof(expanderPins) / sizeof(expanderPins[0]);
ServoServe servoServe(pServos, expanderPins, expanderPinCount);
#endif // USE_SERVO_LIB

#ifdef USE_OLED_LIB
#include "OledWriter.h"
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
OledDisplay oled(display, SCREEN_ADDRESS);
OledWriter oledWriter(oled);
#endif // USE_OLED_LIB

#ifdef USE_LEDSTRIP_LIB
Adafruit_NeoPixel strip19(19, LEDSTRIP_PIN11, NEO_BRG + NEO_KHZ800);
Adafruit_NeoPixel strip15(15, LEDSTRIP_PIN10, NEO_BRG + NEO_KHZ800);
Adafruit_NeoPixel strip4(4, LEDSTRIP_PIN12, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip8(8, LEDSTRIP_PIN13, NEO_GRB + NEO_KHZ800);

// Adafruit_NeoPixel strip16 = Adafruit_NeoPixel(7, LEDSTRIP_PIN16, NEO_BRG + NEO_KHZ800);
LedSegment segs[] = {
    LedSegment(0, 3),   //0 4 led indicators
    LedSegment(4, 45),  //1 8+19
    LedSegment(4, 11),  //2 8
    LedSegment(12, 30), //3 19 a
    LedSegment(31, 45), //4 16
};

size_t ledSegmentCount = sizeof(segs) / sizeof(segs[0]);

Adafruit_NeoPixel *strips[] = {
    &strip4,
    &strip8,
    &strip19,
    &strip15,
};

size_t ledStripCount = sizeof(strips) / sizeof(strips[0]);

LedStrips led(strips, ledStripCount, segs, ledSegmentCount);
LedMenus ledMenus(segs, ledSegmentCount);
#endif // USE_LEDSTRIP_LIB

#ifdef USE_TOUCH_LIB
TouchSensor touch(TOUCH_SENSOR_PIN);
#endif // USE_TOUCH_LIB

#if defined(USE_ROTARY_LIB)
#include "RotaryReader.h"
RotaryEncoder enc(ROTARY_A, ROTARY_B, RotaryEncoder::LatchMode::TWO03);
Rotary rotary(enc, ROTARY_BUTTON);
RotaryReader rotaryReader(rotary);
#endif // USE_ROTARY_LIB

// for ESP32 LED_BUILTIN is defined as static const uint8_t LED_BUILTIN = 2;
#if !defined(LED_BUILTIN) && !defined(ESP32)
#define LED_BUILTIN PB1
#endif
// On the Zero and others we switch explicitly to SerialUSB
#ifdef ARDUINO_ARCH_SAMD
#define Serial SerialUSB
#endif

// only interrupt change pins on nano
// #define SOFT_RX 2
// #define SOFT_TX 3
// SoftwareSerial Serial1(SOFT_RX, SOFT_TX); // RX, TX
char usrBuffer[81] = {0};
char testBuffer[81] = {0};
MicroTerm usrTerm(Serial, usrBuffer, sizeof(usrBuffer));
CameraListener camTerm(Serial1);

//#include <menus.h>
#include "LedMenus.h"
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

#if defined(USE_LEDSTRIP_LIB)
  led.setup();
  ledMenus.initLedMenu(ledSegmentCount);
  Serial.print(F("led strip setup: "));
  Serial.print(led.numPixels());
  Serial.println(F(" pixels defined."));
#endif

#if defined(USE_SERVO_LIB)
  servoServe.setup();
  // initServoMenu();
  Serial.println(F("servo setup"));
  Serial.println(F("servo menu setup"));
#endif

#if defined(USE_OLED_LIB)
  oled.setup();
  if (oled.isActive())
  {
    Serial.println(F("OLED OK!"));
  }
  else
  {
    Serial.println(F("OLED FAILED!"));
  }
  Menu::SetWriter(&oledWriter);
#endif

#if defined(USE_TOUCH_LIB)
  touch.setup();
  Serial.println(F("Touch Sensor OK!"));
  Menu::SetReader(&touchReader);
#endif

#if defined(USE_ROTARY_LIB)
  rotary.setup();
  Serial.println(F("Rotary Switch Setup!"));
  Menu::SetReader(&rotaryReader);
#endif

  usrTerm.setup(".usr");
  Menu::setup();

  Serial1.flush();
  printFree();
}

void processError(int err);
void processBuffer(const char *buf);

void loop()
{
  Menu::loop();

#if defined(USE_SERVO_LIB)
  servoServe.loop();
#endif

#if defined(USE_LEDSTRIP_LIB)
  led.loop();
#endif

  if (usrTerm.ready())
  {
    const char *buf = usrTerm.get();
    processBuffer(buf);
  }

  if (camTerm.ready())
  {
    if (camTerm.hasMessage())
    {
      usrTerm.println(camTerm.get());
      return;
    }

    const char *buf = camTerm.get();
    processBuffer(buf);
  }
}

void processBuffer(const char *buf)
{

  if (buf != NULL)
  {
    char c = buf[0];

#if defined(USE_SERVO_LIB)
    if (c == '?')
    {
      strncpy(testBuffer, buf, sizeof(testBuffer));
      usrTerm.print(F("servo: "));
      usrTerm.println(testBuffer);
      int err = servoServe.process(testBuffer);
      processError(err);
      return;
    }
#endif

#if defined(USE_LEDSTRIP_LIB)
    if (c == '!')
    {
      strncpy(testBuffer, buf, sizeof(testBuffer));
      usrTerm.print(F("led: "));
      usrTerm.println(testBuffer);
      int err = led.process(testBuffer);
      processError(err);
      return;
    }
#endif

#if defined(USE_OLED_LIB)
    if (c == '$')
    {
      strncpy(testBuffer, buf, sizeof(testBuffer));
      usrTerm.print(F("oled: "));
      usrTerm.println(testBuffer);
      int err = oled.process(testBuffer);
      processError(err);
      return;
    }
#endif

    Serial1.println(buf);
  }
}

void processError(int err)
{
  switch (err)
  {
  case ERR_NOT_ENOUGH_ARGS:
    usrTerm.println(F("not enough arguments"));
    return;
  case ERR_NOT_FOUND:
    usrTerm.println(F("command not found"));
    return;
  case ERR_INDEX:
    usrTerm.println(F("index out of range"));
    return;
  case ERR_BAD_VALUE:
    usrTerm.println(F("invalid value"));
    return;
  }
}
#endif
