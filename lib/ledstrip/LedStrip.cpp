// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "LedStrip.h"

LedStrip::LedStrip(Adafruit_NeoPixel **strips, uint8_t nStrips,
                   LedSegment *segs, uint8_t nSegs)
    : strips(strips), nStrips(nStrips),
      segments(segs), nSegments(nSegs)
{
  for (uint8_t i = 0; i < nStrips; i++)
  {
    nPixels += strips[i]->numPixels();
    borders[i] = nPixels;
  }
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

  for (uint8_t i = 0; i < nStrips; i++)
  {
    Adafruit_NeoPixel *strip = strips[i];
    strip->begin();
    strip->setBrightness(50);
    strip->show(); // Initialize all pixels to 'off'
  }

  for (uint8_t i = 0; i < nSegments; i++)
  {
    segments[i].setup(this);
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

void LedStrip::setPixelColor(uint16_t index, uint32_t color)
{
  uint16_t strip_index = index;
  uint16_t border = 0;
  uint8_t i = 0;
  for (; i < nStrips; i++)
  {
    border = borders[i];
    if (index < border)
    {
      break;
    }

    strip_index -= border;
  }

  if (i == nStrips)
  {
    return;
  }

  strips[i]->setPixelColor(strip_index, color);
}

void LedStrip::show(uint16_t begin, uint16_t end)
{
  uint16_t border = 0;
  for (uint8_t i = 0; i < nStrips; i++)
  {
    border = borders[i];
    if (begin < border)
    {
      strips[i]->show();
      if (end < border)
      {
        break;
      }
    }
  }
}

void LedStrip::brightness(uint16_t begin, uint16_t end, uint8_t val)
{
  uint16_t border = 0;
  for (uint8_t i = 0; i < nStrips; i++)
  {
    border = borders[i];
    if (begin < border)
    {
      strips[i]->setBrightness(val);
      strips[i]->show();
      if (end < border)
      {
        break;
      }
    }
  }
}