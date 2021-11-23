// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include <Arduino.h>
#include <MicroTerm.h>
#include "CameraListener.h"
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
  Serial.print("led strip setup: ");
  Serial.print(led.numPixels());
  Serial.println(" pixels defined.");
#endif

#if defined(USE_SERVO_LIB)
  servoServe.setup();
  Serial.println("servo setup");
  usrTerm.setup(".usr");

  servoServe.start();
  Serial1.flush();
#endif

#if defined(USE_OLED_LIB)
  oled.setup();
  if (oled.isActive())
  {
    Serial.println("OLED OK!");
  }
  else
  {
    Serial.println("OLED FAILED!");
  }
#endif

#if defined(USE_TOUCH_LIB)
  touch.setup();
  Serial.println("Touch Sensor OK!");
#endif
}

void processError(int err);
void processBuffer(const char *buf);

////////////////////////////////////////////
#if defined(USE_TOUCH_LIB)
uint8_t pic = 0;
bool blinking = false;

#if defined(USE_OLED_LIB)
void touchTap()
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
#elif defined(USE_LEDSTRIP_LIB)
void touchTap()
{
}
#else
void touchTap()
{
}
#endif
////////////

////////////
#if defined(USE_LEDSTRIP_LIB)
void touchHold()
{
  unsigned parms[1] = {0};
  if (blinking)
  {
    segs[2].start(1, parms, 1);
    blinking = false;
  }
  else
  {
    segs[2].start(8, parms, 1);
    blinking = true;
  }
}
#elif defined(USE_LEDSTRIP_LIB)
void touchHold()
{
}
#else
void touchHold()
{
}
#endif
////////////

void touchMenu()
{
  ActionState state = touch.getState();
  if (state == TOUCH_TAP)
  {
    touchTap();
  }
  else if (state == TOUCH_HOLD)
  {
    touchHold();
  }
}
#endif
////////////////////////////////////////////

void loop()
{
#if defined(USE_TOUCH_LIB)
  touch.loop();
  touchMenu();
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

#include "error.h"
void processError(int err)
{
  switch (err)
  {
  case ERR_NOT_ENOUGH_ARGS:
    usrTerm.println(F("ERR_NOT_ENOUGH_ARGS"));
    return;
  case ERR_NOT_FOUND:
    usrTerm.println(F("ERR_NOT_FOUND"));
    return;
  case ERR_INDEX:
    usrTerm.println(F("ERR_INDEX"));
    return;
  case ERR_BAD_VALUE:
    usrTerm.println(F("ERR_BAD_VALUE"));
    return;
  }
}