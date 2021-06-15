/*  

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
 */

#include <SPI.h>

#include <TFT_eSPI.h> // Hardware-specific library

#include "FS.h"

#include "IconFS.h"

#define TFT_GREY 0x5AEB // New colour
#define FORMAT_SPIFFS_IF_FAILED true


TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

IconFS icon;

void drawXBM(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t fg, uint16_t bg, uint8_t *buf)
{
  tft.drawXBitmap(x, y, buf, w, h, fg, bg);
}

void startICONC(int16_t x, int16_t y, int16_t w, int16_t h)
{
  tft.setAddrWindow(x, y, w, h);
  tft.startWrite();
}
void iconcush(uint16_t c)
{
  tft_Write_16(c);
}
void drawRGBbmp(int16_t x, int16_t y, int16_t w, uint16_t h, uint16_t *buf)
{
  tft.pushImage(x, y, w, h, buf);
}

void setup(void)
{

  Serial.begin(115200);
  if (!SPIFFS.begin())
  {
    Serial.println("SPIFFS Mount Failed");
    return;
  }


  tft.init();
  tft.setRotation(3);
  tft.fillScreen(0x0000);
  pinMode(21, OUTPUT);
  digitalWrite(21, 1);

  icon.begin(&SPIFFS);
  icon.setDisplay(320,240);
  icon.setdrawBitIconCallBack(drawXBM);
  
  icon.setdrawColorIconCallBack(drawRGBbmp);
  icon.setPreWriteColorIconCallBack(startICONC);
  icon.setStreamWriteColorIconCallBack(iconcush);
  icon.setendWriteColorIconCallBack([]()
                                    { tft.endWrite(); });

  icon.openBitFile("/outxbm.bin");
  Serial.printf("XBM\r\nCNT : %d\r\nw : %d h: %d\r\n size : %d\r\n", icon.getBitIconNumber(), icon.getBitIconWidth(), icon.getBitIconHeight(), icon.getBitIconSize());

  icon.openColorFile("/small.bin");
  Serial.printf("COLOR\r\nCNT : %d\r\nw : %d h: %d\r\n size : %d\r\n", icon.getColorIconNumber(), icon.getColorIconWidth(), icon.getColorIconHeight(), icon.getColorIconSize());
}

void loop()
{
icon.SetCursor(0,0);
static int a = 0;
static int b = 0;

tft.setCursor(0,150,2);
tft.printf("a = %d , b = %d     ",a,b);

//icon.setICONColor(rand()%0xffff,rand()%0xffff);
icon.setICONColor(0xffff,0x0000);
icon.drawBitIcon(a);
icon.drawColorIcon(b);



++a;
++b;

a %= icon.getBitIconNumber();
b %= icon.getColorIconNumber();

delay(200);
}
