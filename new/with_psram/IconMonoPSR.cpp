#include "IconMonoPSR.h"

void IconMono::getFileInfo(IconBitInfo_t *r)
{

    r->IconCount = buffer[0];
    r->IconCount <<= 8;
    r->IconCount |= buffer[1];

    r->IconWidth = buffer[2];

    r->IconHeight = buffer[3];

    r->IconSize = buffer[4];
    r->IconSize <<= 8;
    r->IconSize |= buffer[5];

    Serial.printf("C:%d\r\nW:%d\r\nH:%d\r\nL:%d\r\n",r->IconCount,r->IconWidth,r->IconHeight,r->IconSize);
}

void IconMono::_drawBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t *img, uint16_t fg, uint16_t bg)
{
    if (printTo == 1)
    {
        ada->drawXBitmap(x, y, img, w, h, fg);
    }
    else if (printTo == 2)
    {
        tft->drawXBitmap(x, y, img, w, h, fg, bg);
    }
}

uint8_t *IconMono::getIcon(uint16_t index)
{
    return buffer + ((index * Info.IconSize) + ICON_MONO_FILE_HEADER_OFFSET);
}

/*-------------------------------------------------------------------------*/

bool IconMono::begin()
{
    fg = 0xffff;
    bg = 0x0000;
    printTo = 0;

    return true;
}
void IconMono::end()
{
}

void IconMono::setPrintTo(Adafruit_GFX *d)
{
    ada = d;
    printTo = 1;
}

void IconMono::setPrintTo(TFT_eSPI *d)
{
    tft = d;
    printTo = 2;
}

void IconMono::setDisplay(int16_t w, int16_t h)
{
    _W = w;
    _H = h;
}

void IconMono::SetCursor(int16_t x, int16_t y)
{
    _x = x;
    _y = y;
}

void IconMono::printIcon(uint16_t index)
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

void IconMono::printIcon(uint16_t index, int16_t x, int16_t y)
{
    _drawBitmap(x, y, Info.IconWidth, Info.IconHeight, getIcon(index), fg, bg);
}

void IconMono::loadData(uint8_t *dat)
{
    buffer = dat;
    getFileInfo(&Info);
}

void IconMono::setIconColor(uint16_t fg, uint16_t bg)
{
    this->fg = fg;
    this->bg = bg;
}
