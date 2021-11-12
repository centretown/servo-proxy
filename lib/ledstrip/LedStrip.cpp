// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "LedStrip.h"

LedStrip::LedStrip(Adafruit_NeoPixel &strip, LedSegment *segs, uint8_t nSegs)
    : strip(strip), segments(segs), nSegments(nSegs)
{
}

LedStrip::~LedStrip()
{
}

int LedStrip::process(const char *buf)
{
  size_t len = strlen(buf);
  if (len < 1)
  {
    return ERR_STRIP_OK;
  }

  if (buf[0] != '!')
  {
    return ERR_STRIP_OK;
  }

  unsigned parms[4] = {0};
  unsigned command = 0;
  unsigned segment = 0;
  int nitems = sscanf(buf + 1, "%u %u %u %u %u %u",
                      &command,
                      &segment,
                      &parms[0],
                      &parms[1],
                      &parms[2],
                      &parms[3]);
  if (nitems < 2)
  {
    return ERR_STRIP_NOT_ENOUGH_ARGS;
  }

  if (segment >= nSegments)
  {
    return ERR_STRIP_NOT_FOUND;
  }
  segments[segment].start(command, parms, nitems - 2);
  return ERR_STRIP_OK;
}

void LedStrip::setup()
{
// This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined(__AVR_ATtiny85__)
  if (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif

  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'

  for (uint8_t i = 0; i < nSegments; i++)
  {
    segments[i].setup(&strip);
    segments[i].reset();
  }
}

int LedStrip::loop()
{
  for (uint8_t i = 0; i < nSegments; i++)
  {
    segments[i].loop();
  }
  return ERR_STRIP_OK;
}
