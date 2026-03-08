#include <Arduino.h>
#include <DiceClass.h>

int Dice::roll(U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI u8g2) {
  for (int c = size; c >= 0; c = c - speed) {
    u8g2.clearBuffer();
    int num = random(range);
    draw(c, 0, num, u8g2);
    u8g2.sendBuffer();
    delay(1000/60);
    if (c == 0) {
      return num;
    }
  }
};

void Dice::draw(int x, int y, int v, U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI u8g2) {};
