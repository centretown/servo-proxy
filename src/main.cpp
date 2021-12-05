// Copyright (c) 2021 Dave Marsh. See LICENSE.
#ifdef ARDUINO

#include <Arduino.h>
#include <MicroTerm.h>
#include "CameraListener.h"
#include "shared.h"
#include "config.h"

// for ESP32 LED_BUILTIN is defined as static const uint8_t LED_BUILTIN = 2;
#if !defined(LED_BUILTIN) && !defined(ESP32)
#define LED_BUILTIN PB1
#endif
// On the Zero and others we switch explicitly to SerialUSB
#if defined(ARDUINO_ARCH_SAMD)
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
  initLedMenu();
  Serial.print(F("led strip setup: "));
  Serial.print(led.numPixels());
  Serial.println(F(" pixels defined."));
#endif

#if defined(USE_SERVO_LIB)
  servoServe.setup();
  initServoMenu();
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
#endif

#if defined(USE_TOUCH_LIB)
  touch.setup();
  Serial.println(F("Touch Sensor OK!"));
#endif

#if defined(USE_ROTARY_LIB)
  rotary.setup();
  Serial.println(F("Rotary Switch Setup!"));
#endif

  usrTerm.setup(".usr");
  Menu::Start();

  Serial1.flush();
  printFree();
}

void processError(int err);
void processBuffer(const char *buf);

void loop()
{
#if defined(USE_TOUCH_LIB)
  touch.loop();
  touchMenu();
#endif

#if defined(USE_ROTARY_LIB)
  rotary.loop();
  rotaryMenu();
#endif

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
