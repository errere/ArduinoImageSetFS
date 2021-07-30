#include "IconColorPSR.h"

void IconColorPSR::getFileInfo(IconColorPSR::IconColorInfo_t *r)
{
    r->IconCount = buffer[0];

    r->IconWidth = buffer[1];

    r->IconHeight = buffer[2];

    r->IconSize = buffer[3];

    Serial.printf("C:%d\r\nW:%d\r\nH:%d\r\nL:%d\r\n", r->IconCount, r->IconWidth, r->IconHeight, r->IconSize);
}

void IconColorPSR::_drawRGBBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *img)
{
    if (printTo == 1)
    {
        ada->drawRGBBitmap(x, y, img, w, h);
    }
    else if (printTo == 2)
    {
        tft->pushImage(x, y, w, h, img);
    }
}
void IconColorPSR::_drawRGBBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *img, uint16_t alpha)
{
    if (printTo == 1)
    {
        ada->drawRGBBitmap(x, y, img, w, h);
    }
    else if (printTo == 2)
    {
        tft->pushImage(x, y, w, h, img, alpha);
    }
}

////iconIndex->ptr
uint16_t *IconColorPSR::getIcon(uint16_t index)
{
    return buffer + ((index * Info.IconSize) + ICON_COLOR_FILE_HEADER_OFFSET);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IconColorPSR::begin()
{
    printTo = 0;
    IconAlpha = 0x0000;

    return true;
}

void IconColorPSR::end()
{
}

void IconColorPSR::setPrintTo(Adafruit_GFX *d)
{
    ada = d;
    printTo = 1;
}
void IconColorPSR::setPrintTo(TFT_eSPI *d)
{
    tft = d;
    printTo = 2;
}

void IconColorPSR::setDisplay(int16_t w, int16_t h)
{
    _W = w;
    _H = h;
}
void IconColorPSR::SetCursor(int16_t x, int16_t y)
{
    _x = x;
    _y = y;
}

void IconColorPSR::printIcon(uint16_t index)
{
    printIcon(index, _x, _y);

    if (_x + Info.IconWidth > _W)
    {
        _y += Info.IconHeight;
        _x = 0;
    }
    else
    {
        _x += Info.IconWidth;
    }

    if (_y > _H)
        _y = 0;
}
void IconColorPSR::printIcon(uint16_t index, int16_t x, int16_t y)
{
    _drawRGBBitmap(x, y, Info.IconWidth, Info.IconHeight, getIcon(index));
}

void IconColorPSR::loadData(void *dat)
{
    buffer = (uint16_t *)dat;
    for (size_t i = 0; i < 10; i++)
    {
        Serial.println(buffer[i]);
    }

    getFileInfo(&Info);
}

void IconColorPSR::setIconAlpha(uint16_t A)
{
    IconAlpha = A;
}