#ifndef __ICONMONO_PSR_H__
#define __ICONMONO_PSR_H__

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <Arduino.h>

#include "Adafruit_GFX.h"
#include "TFT_eSPI.h"

#define ICON_MONO_FILE_HEADER_OFFSET 6

class IconMono
{
private:
    typedef struct
    {
        uint16_t IconCount;
        uint16_t IconWidth;
        uint16_t IconHeight;
        uint16_t IconSize;
    } IconBitInfo_t;
    /* data */

    int16_t _x, _y, _W, _H; //cursor,hw

    uint8_t *buffer;

    uint16_t fg, bg;

    IconBitInfo_t Info;

    Adafruit_GFX *ada;
    TFT_eSPI *tft;

    int8_t printTo; //0 = na,1 = ada,2 = espi

    void getFileInfo(IconBitInfo_t *r);

    void _drawBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t *img, uint16_t fg, uint16_t bg);

    uint8_t *getIcon(uint16_t index);

public:
    bool begin();
    void end();

    void setPrintTo(Adafruit_GFX *d);
    void setPrintTo(TFT_eSPI *d);

    void setDisplay(int16_t w, int16_t h);
    void SetCursor(int16_t x, int16_t y);
    int16_t getCursorX() { return _x; }
    int16_t getCursorY() { return _y; }

    void printIcon(uint16_t index);
    void printIcon(uint16_t index, int16_t x, int16_t y);

    void loadData(uint8_t *dat);

    void setIconColor(uint16_t fg, uint16_t bg);
};

#endif