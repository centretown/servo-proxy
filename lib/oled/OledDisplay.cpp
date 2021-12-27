#ifdef ARDUINO

#include "OledDisplay.h"
#include "icons.h"

OledDisplay::OledDisplay(Adafruit_SSD1306 &display,
                         uint8_t addr) : display(display), addr(addr)
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
    dartboard();
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
        display.display();
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
    display.begin(SSD1306_SWITCHCAPVCC, addr);
    display.clearDisplay();
}

void OledDisplay::drawText(const char *text, uint8_t x, uint8_t y, uint8_t size, uint16_t color)
{
    reset();
    display.setTextSize(size);
    display.setTextColor(color);
    display.setCursor(x, y);
    display.println(text);
    display.display();
}

void OledDisplay::drawMenu(const char *heading, const char *label)
{
    reset();
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(2, 1);
    display.println(heading);

    display.setTextSize(2);
    display.setCursor(2, 15);
    display.print(label);
    display.display();
}

void OledDisplay::drawMenu(const char *heading, const char *label, int16_t value)
{
    reset();
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(2, 1);
    display.print(heading);
    display.setCursor(80, 1);
    display.print(value);

    // Serial.print(heading);
    // Serial.print(" ");
    // Serial.print(value);

    display.setTextSize(2);
    display.setCursor(2, 15);
    display.print(label);

    int16_t w = display.width();
    int16_t dh = display.height();
    int16_t h = (value * dh) / 255;
    int16_t y = dh - h;

    display.drawRect(w - 20, y, 20, h, SSD1306_WHITE);

    // for (int16_t x = w - 20; x < w; x++)
    // {
    //     display.drawFastVLine(x, y, h, SSD1306_WHITE);
    // }
    display.display();
}

void OledDisplay::drawIcon(IconID id, int16_t x, int16_t y)
{
    // display.invertDisplay(true);

    reset();
    switch (id)
    {
    case ICON_DARTBOARD:
        display.drawXBitmap(display.width() - dartboard_width - 2,
                            display.height() - dartboard_height - 1,
                            dartboard_bits, dartboard_width, dartboard_height,
                            SSD1306_WHITE);
        break;
    case ICON_DOG:
        display.drawXBitmap(display.width() - dog_width - 2,
                            display.height() - dog_height - 1,
                            dog_bits, dog_width, dog_height,
                            SSD1306_WHITE);
        break;
    case ICON_PANDA:
        display.drawXBitmap(display.width() - panda_width - 2,
                            display.height() - panda_height - 1,
                            panda_bits, panda_width, panda_height,
                            SSD1306_WHITE);
        break;
    case ICON_LEDSTRIP:
        display.drawXBitmap(display.width() - ledstrip_width - 2,
                            display.height() - ledstrip_height - 1,
                            ledstrip_bits, ledstrip_width, ledstrip_height,
                            SSD1306_WHITE);
        break;
    case ICON_GEARS:
        display.drawXBitmap(display.width() - gears_width - 2,
                            display.height() - gears_height - 1,
                            gears_bits, gears_width, gears_height,
                            SSD1306_INVERSE);
        break;

    default:
        break;
    }
    display.display();
}

#endif