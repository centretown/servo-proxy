// Copyright (c) 2021 Dave Marsh. See LICENSE.
#ifdef ARDUINO

#include "LedStrips.h"

LedStrips::LedStrips(Adafruit_NeoPixel **neoPix, size_t count,
                     LedSegment *segs, size_t nSegs)
{
  nStrips = count;
  segments = segs;
  nSegments = nSegs;
  nPixels = 0;

  for (uint8_t i = 0; i < nStrips; i++)
  {
    ledStrip *p = strips + i;
    p->strip = neoPix[i];
    p->begin = nPixels;
    nPixels += p->strip->numPixels();
    p->end = nPixels - 1;
  }
}

LedStrips::~LedStrips()
{
}

void LedStrips::setup()
{
// This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined(__AVR_ATtiny85__)
  if (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif

  for (size_t i = 0; i < nStrips; i++)
  {

    ledStrip *p = strips + i;

    p->strip->begin();
    p->strip->clear();
    p->strip->show();

    Serial.print("strip ");
    Serial.print(i);
    Serial.print(" begin ");
    Serial.print(p->begin);
    Serial.print(" end ");
    Serial.println(p->end);
  }

  for (size_t i = 0; i < nSegments; i++)
  {
    segments[i].setWriter(this);
    segments[i].reset();
    Serial.print("segment ");
    Serial.print(i);
    Serial.print(" begin ");
    Serial.print(segments[i].Begin());
    Serial.print(" end ");
    Serial.println(segments[i].End());
  }
}

int LedStrips::process(const char *buf)
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

  unsigned parms[6] = {0};
  unsigned command = 0;
  unsigned segment = 0;
  int nitems = sscanf(buf + 1, "%u %u %u %u %u %u %u %u",
                      &command,
                      &segment,
                      &parms[0],
                      &parms[1],
                      &parms[2],
                      &parms[3],
                      &parms[4],
                      &parms[5]);
  if (nitems < 2)
  {
    return ERR_STRIP_NOT_ENOUGH_ARGS;
  }

  if (command >= STRIP_OUT_OF_BOUNDS)
  {
    return ERR_STRIP_NOT_FOUND;
  }

  if (segment >= nSegments)
  {
    return ERR_STRIP_INDEX;
  }
  segments[segment].start((LedOperator)command, parms, nitems - 2);
  return ERR_STRIP_OK;
}

int LedStrips::loop()
{
  for (uint8_t i = 0; i < nSegments; i++)
  {
    segments[i].tick();
  }
  return ERR_STRIP_OK;
}

void LedStrips::setPixelColor(uint16_t index, uint32_t color)
{
  uint16_t i = 0;
  for (; i < nStrips; i++)
  {
    ledStrip *p = strips + i;
    if (index >= p->begin && index <= p->end)
    {
      p->strip->setPixelColor(index - p->begin, color);
      return;
    }
  }
}

void LedStrips::show(uint16_t begin, uint16_t end)
{
  uint16_t index = begin;
  for (uint16_t i = 0; i < nStrips; i++)
  {
    ledStrip *p = strips + i;
    if (index > p->end)
    {
      continue;
    }

    p->strip->show();
    index = p->end + 1;
  }
}

#endif
