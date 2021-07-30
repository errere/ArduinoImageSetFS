#ifndef __ICON_COLOR_PSR_H__
#define __ICON_COLOR_PSR_H__

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <Arduino.h>

#include "Adafruit_GFX.h"
#include "TFT_eSPI.h"
#define ICON_COLOR_FILE_HEADER_OFFSET 8
class IconColorPSR
{
private:
    typedef struct
    {
        uint16_t IconCount;
        uint16_t IconWidth;
        uint16_t IconHeight;
        uint16_t IconSize;
    } IconColorInfo_t;

    uint16_t *buffer;

    int16_t _x, _y, _W, _H; //cursor,hw

    IconColorInfo_t Info;

    Adafruit_GFX *ada;
    TFT_eSPI *tft;

    int8_t printTo; //0 = na,1 = ada,2 = espi

    uint16_t IconAlpha;

    void getFileInfo(IconColorInfo_t *r);

    void _drawRGBBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *img);
    void _drawRGBBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *img, uint16_t alpha);

    uint16_t *getIcon(uint16_t index); //iconIndex->ptr

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

    void loadData(void *dat);

    void setIconAlpha(uint16_t A);
};

#endif