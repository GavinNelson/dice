#ifndef D6_h
#define D6_h

#include <DiceClass.h>
#include <U8g2lib.h>
#include <Arduino.h>

class D6: public Dice {
  int dotsize;
  int quart;
  int threequart;
  int half;
  public:
    int range = 5;
    D6(int s) {
      size = s;
      dotsize = size/15;
      quart = size/4;
      threequart = 3*quart;
      half = size/2;
    };
    void draw(int x, int y, int v, U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI u8g2) {
      u8g2.drawFrame(x, y, size, size);
      if (v % 2) {
        //center
        u8g2.drawDisc(x + half, y + half, dotsize, U8G2_DRAW_ALL);
      }
      if (v > 1) {
        //top left + bottom right
        u8g2.drawDisc(x + quart, y + quart, dotsize, U8G2_DRAW_ALL);
        u8g2.drawDisc(x + threequart, y + threequart, dotsize, U8G2_DRAW_ALL);
      }
      if (v > 3) {
        //top right + bottom left
        u8g2.drawDisc(x + threequart, y + quart, dotsize, U8G2_DRAW_ALL);
        u8g2.drawDisc(x + quart, y + threequart, dotsize, U8G2_DRAW_ALL);
      }
      if (v == 6) {
        //left and right
        u8g2.drawDisc(x + quart, y + half, dotsize, U8G2_DRAW_ALL);
        u8g2.drawDisc(x + threequart, y + half, dotsize, U8G2_DRAW_ALL);
      }
    };
};

#endif
