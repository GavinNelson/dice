#ifndef DiceClass_h
#define DiceClass_h

#include <U8g2lib.h>
#include <Arduino.h>

class Dice {
  public:
    int size;
    int range;
    int speed = 4;
    // Dice(int s) {
    //   size = s;
    // }
    int roll(U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI u8g2);
    void draw(int x, int y, int v, U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI u8g2);
};

#endif
