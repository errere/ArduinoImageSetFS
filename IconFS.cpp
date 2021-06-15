#include "IconFS.h"

/*==============================================================================public==============================================================================*/
uint8_t IconFS::begin(fs::FS *f)
{

    _fs = f;
    _x = 0;
    _y = 0;
    _LCD_W = 0;
    _LCD_H = 0;

#ifdef USE_COLOR_ICON
    if (COLORICON_CACHE_MAX_RES != 0)
        ColorBuffer = (uint16_t *)malloc(COLORICON_CACHE_MAX_RES * sizeof(uint16_t));
    ColorFileisOpened = 0;
#endif

#ifdef USE_BIT_ICON
    BitFileisOpened = 0;
    fg = 0xffff;
    bg = 0x0000;
#endif
    return 1;
}

uint8_t IconFS::end()
{
#ifdef USE_COLOR_ICON
    free(ColorBuffer);
#endif
    return 0;
}

/*==============================================color==============================================*/
#ifdef USE_COLOR_ICON

void IconFS::setdrawColorIconCallBack(drawColorIconCallBack_t *cb)
{
    drawColorIconCallBack = cb;
}
void IconFS::setPreWriteColorIconCallBack(PreWriteColorIconCallBack_t *cb)
{
    PreWriteColorIcon = cb;
}
void IconFS::setStreamWriteColorIconCallBack(StreamWriteColorIconCallBack_t *cb)
{
    _writePixel = cb;
}
void IconFS::setendWriteColorIconCallBack(endWriteColorIconCallBack_t *cb)
{
    endWriteColorIcon = cb;
}

uint8_t IconFS::openColorFile(const char *dir)
{
    if (!ColorFileisOpened)
    {
        _fileColor = _fs->open(dir, "r");
        ColorFileisOpened = (_fileColor) ? 1 : 0;
        if (_fileColor)
            getColorFileInfo(&Colorinfo);
        return _fileColor;
    }
    return 0xff;
}
void IconFS::closeColorFile()
{
    if (ColorFileisOpened)
    {
        _fileColor.close();
        ColorFileisOpened = 0;
    }
}

uint16_t IconFS::getColorIconNumber()
{
    return Colorinfo.IconCount;
}
uint16_t IconFS::getColorIconWidth()
{
    return Colorinfo.IconWidth;
}
uint16_t IconFS::getColorIconHeight()
{
    return Colorinfo.IconHeight;
}
uint16_t IconFS::getColorIconSize()
{
    return Colorinfo.IconSize;
}

void IconFS::drawColorIcon(int16_t x, int16_t y, uint16_t index)
{
    _fileColor.seek((index * Colorinfo.IconSize) + HEADER_COLOR);
    //Serial.println("dc");
    if (Colorinfo.IconSize > (COLORICON_CACHE_MAX_RES * 2))
    {
        //Serial.println("s");
        PreWriteColorIcon(x, y, Colorinfo.IconWidth, Colorinfo.IconHeight);
        uint16_t temp;
        for (uint32_t i = 0; i < Colorinfo.IconSize/2; i++)
        {
            temp = _fileColor.read();
            temp <<= 8;
            temp |= _fileColor.read();
            _writePixel(temp);
        }
        endWriteColorIcon();

    } //stream
    else
    {
        //Serial.println("b");
        uint16_t temp;
        for (uint32_t i = 0; i < Colorinfo.IconSize/2; i++)
        {
            temp = _fileColor.read();
            temp |= _fileColor.read()<<8;
            ColorBuffer[i] = temp;
        }
        drawColorIconCallBack(x, y, Colorinfo.IconWidth, Colorinfo.IconHeight, ColorBuffer);
    } //buffer
} //draw

void IconFS::drawColorIcon(uint16_t index)
{
    drawColorIcon(_x,_y,index);
    if (_x + Colorinfo.IconWidth > _LCD_W)
    {
        _y += Colorinfo.IconHeight;
        _x = 0;
    }
    else
    {
        _x += Colorinfo.IconWidth;
    }
} //draw

#endif //USE_COLOR_ICON

/*==============================================bit==============================================*/
#ifdef USE_BIT_ICON

void IconFS::setdrawBitIconCallBack(drawBitIconCallBack_t *cb)
{
    drawBitIconCallBack = cb;
}

uint8_t IconFS::openBitFile(const char *dir)
{
    if (!BitFileisOpened)
    {
        _file = _fs->open(dir, "r");
        BitFileisOpened = (_file) ? 1 : 0;
        if (_file)
            getBitFileInfo(&BitInfo);
        return _file;
    }
    return 0xff;
}
void IconFS::closeBitFile()
{
    if (BitFileisOpened)
    {
        _file.close();
        BitFileisOpened = 0;
    }
}

uint16_t IconFS::getBitIconNumber()
{
    return BitInfo.IconCount;
}
uint16_t IconFS::getBitIconWidth()
{
    return BitInfo.IconWidth;
}
uint16_t IconFS::getBitIconHeight()
{
    return BitInfo.IconHeight;
}
uint16_t IconFS::getBitIconSize()
{
    return BitInfo.IconSize;
}

void IconFS::drawBitIcon(int16_t x, int16_t y, uint16_t index)
{
    _file.seek((index * BitInfo.IconSize) + HEADER_BIT);
    if (BitBuffer)
    {
        free(BitBuffer);
        BitBuffer = nullptr;
    }

    BitBuffer = (uint8_t *)malloc(BitInfo.IconSize);
    for (uint32_t i = 0; i < BitInfo.IconSize; i++)
    {
        BitBuffer[i] = _file.read();
    }
    drawBitIconCallBack(x, y, BitInfo.IconWidth, BitInfo.IconHeight, fg, bg, BitBuffer);

    free(BitBuffer);
    BitBuffer = nullptr;
}
void IconFS::drawBitIcon(uint16_t index)
{
    drawBitIcon(_x,_y,index);

    if (_x + BitInfo.IconWidth > _LCD_W)
    {
        _y += BitInfo.IconHeight;
        _x = 0;
    }
    else
    {
        _x += BitInfo.IconWidth;
    }
}

#endif //USE_BIT_ICON

/*==============================================================================PRIVATE==============================================================================*/

#ifdef USE_COLOR_ICON
/*
    byteorder = "big"
	typedef struct {
        
		uint16_t icon_cnt;
		uint16_t icon_SizeW;
        uint16_t icon_SizeH;
		uint16_t icon_length;
		uint8_t** icon;

	}icon_struct;
*/
void IconFS::getColorFileInfo(IconColorInfo_t *r)
{
    _fileColor.seek(0);

    r->IconCount = _fileColor.read();
    r->IconCount <<= 8;
    r->IconCount |= _fileColor.read();

    r->IconWidth = _fileColor.read();
    r->IconWidth <<= 8;
    r->IconWidth |= _fileColor.read();

    r->IconHeight = _fileColor.read();
    r->IconHeight <<= 8;
    r->IconHeight |= _fileColor.read();

    r->IconSize = _fileColor.read();
    r->IconSize <<= 8;
    r->IconSize |= _fileColor.read();
}
#endif //USE_COLOR_ICON

#ifdef USE_BIT_ICON

/*
    byteorder = "big"
	typedef struct {
        
		uint16_t icon_cnt;
		uint8_t icon_Size_W;
        uint8_t icon_Size_H;
		uint16_t icon_length;
		uint8_t** icon;

	}icon_struct;
*/
void IconFS::getBitFileInfo(IconBitInfo_t *r)
{
    _file.seek(0);

    r->IconCount = _file.read();
    r->IconCount <<= 8;
    r->IconCount |= _file.read();

    r->IconWidth = _file.read();

    r->IconHeight = _file.read();

    r->IconSize = _file.read();
    r->IconSize <<= 8;
    r->IconSize |= _file.read();
}
#endif //USE_BIT_ICON
