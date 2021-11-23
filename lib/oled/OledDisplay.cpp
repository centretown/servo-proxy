#include "OledDisplay.h"

#define dog_width 128
#define dog_height 32
static const uint8_t PROGMEM dog_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0x0f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0xe0, 0xeb, 0x5f, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0x0f, 0x00, 0x00, 0x00, 0x70, 0x00, 0x0a, 0xf0, 0x0b, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x70, 0x55, 0x80, 0xe3,
    0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00,
    0xf0, 0xe0, 0x08, 0x07, 0x38, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0xe0,
    0x07, 0x00, 0x00, 0x00, 0xe0, 0x7f, 0x00, 0x00, 0x1e, 0x78, 0x1f, 0x00,
    0x00, 0x00, 0x00, 0xe0, 0x0e, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x12, 0xd0,
    0x0f, 0x1c, 0x38, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x00, 0x00, 0x00,
    0x3f, 0x85, 0x40, 0xfc, 0x03, 0x1c, 0x38, 0x00, 0x00, 0x00, 0x00, 0xe0,
    0xf8, 0xaa, 0xaa, 0xfe, 0x07, 0x00, 0x04, 0xf0, 0x01, 0x1c, 0x38, 0x00,
    0x00, 0x00, 0x00, 0xe0, 0xe1, 0xff, 0xff, 0x3f, 0x40, 0x24, 0xa9, 0xff,
    0x01, 0x78, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x20, 0x00,
    0x80, 0x00, 0xe8, 0x7f, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x80,
    0x87, 0x24, 0x02, 0x41, 0x12, 0x04, 0xfd, 0x09, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x1f, 0x40, 0x08, 0x08, 0x00, 0x48, 0xfe, 0xff,
    0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0x01, 0x20, 0x42,
    0x44, 0xe0, 0x7f, 0xed, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaf,
    0x27, 0x08, 0x02, 0x00, 0x00, 0xc1, 0x07, 0x01, 0xfa, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x97, 0x07, 0x21, 0x48, 0x44, 0x04, 0x88, 0xff, 0xbe,
    0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xf3, 0x0f, 0x80, 0x38, 0x80,
    0x90, 0x00, 0xfc, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf9, 0xfc,
    0x23, 0x09, 0xbf, 0x0a, 0x7e, 0x12, 0x01, 0xd0, 0x3f, 0x00, 0x00, 0x00,
    0x00, 0xf0, 0x3f, 0x1f, 0x00, 0xa0, 0xff, 0xab, 0xf8, 0x27, 0x20, 0x01,
    0xf8, 0x01, 0x00, 0x00, 0x00, 0x78, 0xd5, 0x07, 0x4a, 0xd5, 0xe3, 0xff,
    0xff, 0x5f, 0x55, 0xbf, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x1e, 0xc9, 0xa3,
    0x5a, 0xff, 0x00, 0xf4, 0xff, 0xf8, 0xff, 0xff, 0xfb, 0x01, 0x00, 0x00,
    0x00, 0xfe, 0xff, 0xe9, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x40, 0x05, 0xc0,
    0x7f, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x7f, 0xf2, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x3e,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xf8, 0xd2, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x29, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define dartbd2_width 128
#define dartbd2_height 32
static const uint8_t PROGMEM dartbd2_bits[] = {
    0xfe, 0x7f, 0x00, 0x80, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x5f, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0x01, 0x20, 0xe0, 0xff, 0xff, 0xff,
    0xff, 0x02, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0x01, 0xe0,
    0x87, 0xfe, 0xff, 0xff, 0x7f, 0x28, 0xe8, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0xff, 0x07, 0x80, 0x7f, 0xf4, 0xf8, 0xff, 0xbf, 0x42, 0xd1, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x04, 0xfc, 0x0f, 0xc0, 0xff, 0x03, 0xf0, 0xff,
    0x1f, 0x14, 0xc4, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xe0, 0x1f, 0x00,
    0xff, 0x07, 0xf8, 0xff, 0x5f, 0xa1, 0xc8, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x05, 0x50, 0x00, 0x00, 0xfe, 0x07, 0xc0, 0xff, 0x0f, 0x04, 0xe1, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x2b, 0x00, 0x00, 0x00, 0xf4, 0x07, 0x00, 0xbf,
    0x5f, 0x29, 0xf2, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0x00, 0x01, 0x5c,
    0x80, 0x06, 0x00, 0x04, 0x8f, 0x44, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x3f, 0xe0, 0x03, 0xf0, 0x03, 0x00, 0x80, 0x03, 0x94, 0x94, 0xfe, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x1f, 0xd0, 0x07, 0xe0, 0x03, 0x00, 0x80, 0x4f,
    0x70, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1d, 0x80, 0x3e, 0x80,
    0x07, 0x18, 0xc0, 0xbf, 0xe2, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x0a, 0x00, 0x7a, 0x00, 0x0f, 0xf0, 0xe0, 0x1f, 0xf4, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x08, 0x00, 0xc0, 0x01, 0x0c, 0xf0, 0x07, 0xa7,
    0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0x80, 0x07,
    0x1c, 0xf0, 0x07, 0xd0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x0e, 0x00, 0x1c, 0x30, 0xf0, 0x03, 0xe5, 0x03, 0xbc, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x00, 0xfe, 0x00, 0x70, 0x60, 0xf0, 0x01, 0x20,
    0x00, 0x88, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x09, 0xfe, 0x1f, 0x80,
    0x41, 0xf0, 0x00, 0x10, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x15, 0xa8, 0xff, 0x01, 0x86, 0xd0, 0x00, 0x3e, 0x00, 0x80, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x1f, 0x00, 0xa0, 0x3f, 0x90, 0x31, 0x80, 0x7f,
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x5f, 0x02, 0x00, 0xd0,
    0x61, 0x3b, 0xf0, 0xff, 0x01, 0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x5f, 0x00, 0x00, 0x00, 0x6a, 0x25, 0xfe, 0x5f, 0x01, 0x52, 0xf9, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0xc0, 0xa8, 0x01, 0x00,
    0x80, 0xff, 0x71, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x02, 0x00, 0x00,
    0xa9, 0x4b, 0x07, 0x00, 0x00, 0xfe, 0x07, 0xf8, 0xff, 0xff, 0xff, 0xff,
    0x95, 0x00, 0xfd, 0xff, 0x1f, 0xd4, 0x55, 0x00, 0x00, 0xfc, 0x07, 0xf0,
    0xff, 0xff, 0xff, 0xff, 0x01, 0x04, 0xb7, 0xb6, 0x02, 0xaa, 0x07, 0x5f,
    0x00, 0xf8, 0x1f, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x04, 0x00, 0xfc, 0xff,
    0x00, 0x8e, 0x08, 0xf0, 0x5f, 0xa1, 0x3e, 0x7a, 0xff, 0xff, 0xff, 0xff,
    0x50, 0x00, 0xf0, 0x0f, 0x00, 0x0d, 0x61, 0x00, 0xfc, 0x01, 0x20, 0xfc,
    0xfe, 0xff, 0xff, 0xff, 0x0a, 0x01, 0xd0, 0x01, 0x80, 0x06, 0x83, 0x01,
    0xc0, 0x03, 0x00, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x52, 0x2a, 0x20, 0x00,
    0xc0, 0x0f, 0x0e, 0x0e, 0x00, 0x02, 0x00, 0xf0, 0xfb, 0xff, 0xff, 0xff,
    0x8a, 0x80, 0x00, 0x00, 0xc0, 0x06, 0x14, 0x38, 0x00, 0x00, 0x00, 0xf0,
    0xff, 0xff, 0xff, 0xff, 0x32, 0x15, 0x3f, 0x00, 0xf0, 0x0f, 0x38, 0xe0,
    0x01, 0x00, 0x00, 0xe0, 0xfb, 0xff, 0xff, 0xff};

#define panda_width 128
#define panda_height 32
static const uint8_t PROGMEM panda_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x80, 0x04, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x01, 0x00,
    0x00, 0x80, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
    0xff, 0xff, 0x0f, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x7f, 0x00, 0x00, 0xfc, 0xff, 0xff,
    0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8,
    0xff, 0xff, 0x1f, 0x00, 0x00, 0xe0, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff,
    0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x1f, 0x00, 0x00,
    0x00, 0x00, 0xf0, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc,
    0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0x1f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xf8, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe,
    0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x3f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
    0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
    0x01, 0x80, 0x3f, 0x00, 0x00, 0xff, 0x01, 0x00, 0x07, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xe0, 0x00, 0x78, 0xf8, 0x03, 0xc0, 0x3f, 0x1e, 0x00,
    0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x3c, 0xf0, 0x03,
    0xc0, 0x0f, 0x3c, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38,
    0x00, 0x3f, 0xe0, 0x01, 0x80, 0x4f, 0x78, 0x00, 0x18, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x38, 0x00, 0x3f, 0x77, 0x00, 0x00, 0xce, 0xfd, 0x00,
    0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0xff, 0x7a, 0x00,
    0x40, 0xbc, 0xfe, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38,
    0x00, 0xff, 0x07, 0xf4, 0x2f, 0xe0, 0x7f, 0x00, 0x30, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x38, 0x00, 0xbe, 0x01, 0xfe, 0x7f, 0x80, 0x3a, 0x00,
    0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0xc0,
    0x03, 0x00, 0x02, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
    0x00, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xc0, 0x01, 0x20, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00,
    0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x21,
    0x03, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1c, 0x80, 0x00, 0xe8, 0x1f, 0x00, 0x02, 0x70, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x0e,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xf0, 0x02, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0x12, 0xa4, 0xfe, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf4,
    0xaf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

OledDisplay::OledDisplay(Adafruit_SSD1306 &display, uint8_t addr) : display(display), addr(addr)
{
}

OledDisplay::~OledDisplay()
{
}

void OledDisplay::setup()
{
    active = display.begin(SSD1306_SWITCHCAPVCC, addr);
    if (!active)
    {
        return;
    }

    display.clearDisplay();
    dog();
}

void OledDisplay::drawBitmap()
{
}

void OledDisplay::loop()
{
}

int OledDisplay::process(const char *buf)
{
    size_t len = strlen(buf);
    if (len < 1)
    {
        return ERR_OLED_OK;
    }

    if (buf[0] != '$')
    {
        return ERR_OLED_OK;
    }

    char parms[32] = {0};
    unsigned command = 0;
    int nitems = sscanf(buf + 1, "%u %32s", &command, &parms);
    Serial.println(parms);
    start(command, parms, nitems);
    return ERR_OLED_OK;
}

void OledDisplay::start(uint8_t command, char *parms, size_t nparms)
{
    switch (command)
    {
    case OLED_RESET:
        reset();
        break;
    case OLED_TEXT:
        if (nparms > 1)
        {
            drawText(parms);
        }
        break;
    case OLED_DARTBOARD:
        dartboard();
        break;
    case OLED_DOG:
        dog();
        break;
    case OLED_PANDA:
        panda();
        break;

    default:
        break;
    }
}

void OledDisplay::reset()
{
    display.clearDisplay();
    display.display();
}

void OledDisplay::drawText(const char *text, uint8_t x, uint8_t y, uint8_t size, uint16_t color)
{
    display.clearDisplay();
    display.setTextSize(size);
    display.setTextColor(color);
    display.setCursor(x, y);
    display.println(text);
    display.display();
}

void OledDisplay::dog()
{
    display.clearDisplay();
    display.invertDisplay(true);
    display.drawXBitmap(0, 0, dog_bits, dog_width, dog_height, SSD1306_INVERSE);
    display.display();
}
void OledDisplay::dartboard()
{
    display.clearDisplay();
    display.invertDisplay(true);
    display.drawXBitmap(0, 0, dartbd2_bits, dartbd2_width, dartbd2_height, SSD1306_WHITE);
    display.display();
}
void OledDisplay::panda()
{
    display.clearDisplay();
    display.invertDisplay(true);
    display.drawXBitmap(0, 0, panda_bits, panda_width, panda_height, SSD1306_INVERSE);
    display.display();
}