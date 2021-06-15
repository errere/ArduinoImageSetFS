#ifndef __ICONFS_H__
#define __ICONFS_H__

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <Arduino.h>
#include "FS.h"

#define USE_COLOR_ICON
#define USE_BIT_ICON

#define COLORICON_CACHE_MAX_RES 512 // W*H < 500 aka 1KB RAM used for cache

#define HEADER_COLOR 8
#define HEADER_BIT 6

class IconFS
{
private:
    fs::FS *_fs;

    int16_t _x, _y, _LCD_W, _LCD_H;
/*==============================================color==============================================*/
#ifdef USE_COLOR_ICON

    typedef struct
    {
        uint16_t IconCount;
        uint16_t IconWidth;
        uint16_t IconHeight;
        uint16_t IconSize;
    } IconColorInfo_t;

    fs::File _fileColor;
    uint8_t ColorFileisOpened;
    uint16_t *ColorBuffer = nullptr;

    IconColorInfo_t Colorinfo;

    //void callback(x,y,w,h,buf);
    using drawColorIconCallBack_t = void(int16_t, int16_t, int16_t, uint16_t, uint16_t *);
    drawColorIconCallBack_t *drawColorIconCallBack;
    //void cb(x,y,w,h)
    using PreWriteColorIconCallBack_t = void(int16_t, int16_t, int16_t, int16_t);
    PreWriteColorIconCallBack_t *PreWriteColorIcon;
    //void cb(color)
    using StreamWriteColorIconCallBack_t = void(uint16_t);
    StreamWriteColorIconCallBack_t *_writePixel;
    //void cb(void)
    using endWriteColorIconCallBack_t = void(void);
    endWriteColorIconCallBack_t *endWriteColorIcon;

    void getColorFileInfo(IconColorInfo_t *r);

#endif //USE_COLOR_ICON
/*==============================================bit==============================================*/
#ifdef USE_BIT_ICON

    typedef struct
    {
        uint16_t IconCount;
        uint16_t IconWidth;
        uint16_t IconHeight;
        uint16_t IconSize;
    } IconBitInfo_t;

    fs::File _file;
    uint8_t BitFileisOpened;
    uint8_t *BitBuffer = nullptr;
    uint16_t fg, bg;

    IconBitInfo_t BitInfo;

    //void cb(x,y,w,h,fg,bg,buf)
    using drawBitIconCallBack_t = void(int16_t, int16_t, int16_t, int16_t, uint16_t, uint16_t, uint8_t *);
    drawBitIconCallBack_t *drawBitIconCallBack;

    void getBitFileInfo(IconBitInfo_t *r);
#endif //USE_BIT_ICON
/*==============================================================================public==============================================================================*/
public:
    uint8_t begin(fs::FS *f);
    uint8_t end();
    void setDisplay(int16_t w, int16_t h)
    {
        _LCD_W = w;
        _LCD_H = h;
    }

    void SetCursor(int16_t x, int16_t y)
    {
        _x = x;
        _y = y;
    }
    int16_t getCursorX() { return _x; }
    int16_t getCursorY() { return _y; }
/*==============================================color==============================================*/
#ifdef USE_COLOR_ICON

    void setdrawColorIconCallBack(drawColorIconCallBack_t * cb);
    void setPreWriteColorIconCallBack(PreWriteColorIconCallBack_t * cb);
    void setStreamWriteColorIconCallBack(StreamWriteColorIconCallBack_t * cb);
    void setendWriteColorIconCallBack(endWriteColorIconCallBack_t * cb);

    uint8_t openColorFile(const char *dir);
    void closeColorFile();

    uint16_t getColorIconNumber();
    uint16_t getColorIconWidth();
    uint16_t getColorIconHeight();
    uint16_t getColorIconSize();

    void drawColorIcon(int16_t, int16_t, uint16_t);
    void drawColorIcon(uint16_t);

#endif //USE_COLOR_ICON
/*==============================================bit==============================================*/
#ifdef USE_BIT_ICON

    void setdrawBitIconCallBack(drawBitIconCallBack_t * cb);

    uint8_t openBitFile(const char *dir);
    void closeBitFile();

    uint16_t getBitIconNumber();
    uint16_t getBitIconWidth();
    uint16_t getBitIconHeight();
    uint16_t getBitIconSize();

    void drawBitIcon(int16_t, int16_t, uint16_t);
    void drawBitIcon(uint16_t);

    void setICONColor(uint16_t f, uint16_t b)
    {
        fg = f;
        bg = b;
    }

#endif //USE_BIT_ICON
};

#endif

/*
#ifdef USE_COLOR_ICON

#endif //USE_COLOR_ICON
*/
