#include <U8g2lib.h>
#include <Wire.h>

int i = 2;
int j = 16;
int c = 10;
int result = 0;
int type = 20;
int speed = 4;
int types[7] = {20,12,10,100,8,6,4};
int rollType = 20;
int time = 0;
float pulseSpeed = 0.05;


U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, 19, 18,0); // pin remapping with ESP8266 HW I2C

void setup(void) {
  Wire.setSDA(17);
  Wire.setSCL(16);
  Serial.begin(9600);
  u8g2.begin();
  randomSeed(analogRead(0));
  u8g2.setFontPosCenter();
  pinMode(13, OUTPUT);
  pinMode(21, INPUT_PULLUP);
}

void loop(void) {
  int ti = getTypeIndex();
  type = types[ti];
  u8g2.clearBuffer();
  if (digitalRead(21) == HIGH) {
    digitalWrite(13, HIGH);
    rollType = type;
    u8g2.clearBuffer();
    result = roll(type);
    u8g2.sendBuffer();
  } else {
    digitalWrite(13, LOW);
  }
  if (result) {
    drawDie(rollType, 0, 0, 64, result);
    drawSelect(ti);
    u8g2.sendBuffer();
  } else {
    splash();
  }
  time++;
}

void splash() {
  u8g2.drawDisc(64, 32, 16 + sin(time * pulseSpeed) * 16);
  u8g2.sendBuffer();
}

int getTypeIndex() {
  int i = analogRead(15)* -1;
  return map(i+1024, 0, 1024, 0, 6);
}

void drawSelect(int pos) {
  u8g2.setFont(u8g2_font_profont12_tf);
  for (int i = 0; i <= 6; i++) {
    u8g2.setCursor(70, 6 + (9 * i));
    if (i == pos) {
      u8g2.print('[');
    } else {
      u8g2.print(' ');
    }
    u8g2.print('d');
    u8g2.print(types[i]);
    if (i == pos) {
      u8g2.setCursor(100, 6 + (9 * i));
      u8g2.print(']');
    }
  }
}

void drawDie(int type, int x, int y, int size, int num) {
  if (type == 10 || type == 100 || type == 20) {
    u8g2.setFont(u8g2_font_logisoso16_tr);
  } else if (type == 12 || type == 10) {
    u8g2.setFont(u8g2_font_logisoso24_tr);
  } else {
    u8g2.setFont(u8g2_font_logisoso32_tr);
  }
  switch(type) {
    case 20:
      drawD20(x, y, size, num);
      break;
    case 12:
      drawD12(x, y, size, num);
      break;
    case 10:
      drawD10(x, y, size, num, false);
      break;
    case 100:
      drawD100(x, y, size, num);
      break;
    case 8:
      drawD8(x, y, size, num);
      break;
    case 6:
      drawD6(x, y, size, num);
      break;
    case 4:
      drawD4(x,y, size, num);
      break;
  }
}

int roll(int type) {
  int x = 0;
  int y = 0;
  int size = 64;
  for (int c = size; c >= 0; c = c - speed) {
    u8g2.clearBuffer();
    int num = random(type);
    // num = 19;
    if (type != 10 && type != 100) {
      num++;
    }
    drawDie(type, x+c, y, size, num);
    u8g2.sendBuffer();
    delay(1000/60);
    if (digitalRead(2) == HIGH) {
      c += speed;
    }
    if (c == 0) {
      Serial.print(num);
      Serial.print(" ");
      if (type == 20 && num == 20) {
        // crit
        crit();
      }
      return num;
    }
  }
  i++;
  j++;
  u8g2.sendBuffer();
}

void crit() {
  for (int i = 0; i <= 128; i = i + speed) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_logisoso50_tr);
    u8g2.setCursor(i, 42);
    u8g2.print(20);
    u8g2.sendBuffer();
  }
}

void drawD6(int x, int y, int size, int num) {
  int dotsize = size/15;
  u8g2.drawFrame(x, y, size, size);
  if (num == 1 || num == 3 || num == 5) {
    //center
    u8g2.drawDisc(x + (size/2), y + (size/2), dotsize, U8G2_DRAW_ALL);
  }
  if (num > 1) {
    //top left + bottom right
    u8g2.drawDisc(x + (size/4), y + (size/4), dotsize, U8G2_DRAW_ALL);
    u8g2.drawDisc(x + 3*(size/4), y + 3*(size/4), dotsize, U8G2_DRAW_ALL);
  }
  if (num > 3) {
    //top right + bottom left
    u8g2.drawDisc(x + 3*(size/4), y + (size/4), dotsize, U8G2_DRAW_ALL);
    u8g2.drawDisc(x + (size/4), y + 3*(size/4), dotsize, U8G2_DRAW_ALL);
  }
  if (num == 6) {
    //left and right
    u8g2.drawDisc(x + (size/4), y + (size/2), dotsize, U8G2_DRAW_ALL);
    u8g2.drawDisc(x + 3*(size/4), y + (size/2), dotsize, U8G2_DRAW_ALL);
  }
}

void drawD4(int x, int y, int size, int num) {
  //draw outline
  drawTriLine(x, y, size);

  //put number on
  u8g2.setFontPosCenter();
  char cstr[16];
  itoa(num, cstr, 10);

  int nw = numWidth(num);

  u8g2.setCursor(x + ((size/2) - (nw/2)), 1 + (size*0.6));
  u8g2.print(num);
}

void drawD12(int x, int y, int size, int num) {
  //draw a 10 sided shape around
  int side = size * 0.32495;
  int w = size*0.95;
  int x0 = x + w*0.5;
  int y0 = y;
  int x1 = x + w*0.8;
  int y1 = y + size*0.1;
  int x2 = x + w;
  int y2 = y + (size/2) - (side/2);
  int x3 = x2;
  int y3 = y + (size/2) + (side/2);
  int x4 = x1;
  int y4 = y + size*0.9;
  int x5 = x0;
  int y5 = y + size - 1;
  int x6 = x + w*0.2;
  int y6 = y4;
  int x7 = x;
  int y7 = y3;
  int x8 = x7;
  int y8 = y2;
  int x9 = x6;
  int y9 = y1;

  int x10 = x0;
  int y10 = y + size*0.15;
  int x11 = x + w * 0.85;
  int y11 = y + size*0.4;
  int x12 = x + w * 0.7;
  int y12 = y + size*0.75;
  int x13 = x + w * 0.3;
  int y13 = y12;
  int x14 = x + w*0.15;
  int y14 = y11;
  u8g2.drawLine(x0,y0,x1,y1);
  u8g2.drawLine(x1,y1,x2,y2);
  u8g2.drawLine(x2,y2,x3,y3);
  u8g2.drawLine(x3,y3,x4,y4);
  u8g2.drawLine(x4,y4,x5,y5);
  u8g2.drawLine(x5,y5,x6,y6);
  u8g2.drawLine(x6,y6,x7,y7);
  u8g2.drawLine(x7,y7,x8,y8);
  u8g2.drawLine(x8,y8,x9,y9);
  u8g2.drawLine(x9,y9,x0,y0);
  // draw a hexagon
  u8g2.drawLine(x10,y10,x11,y11);
  u8g2.drawLine(x11,y11,x12,y12);
  u8g2.drawLine(x12,y12,x13,y13);
  u8g2.drawLine(x13,y13,x14,y14);
  u8g2.drawLine(x14,y14,x10,y10);
  //connect hex to 10side
  u8g2.drawLine(x0,y0,x10,y10);
  u8g2.drawLine(x2,y2,x11,y11);
  u8g2.drawLine(x4,y4,x12,y12);
  u8g2.drawLine(x6,y6,x13,y13);
  u8g2.drawLine(x8,y8,x14,y14);

  //draw number
  int nw = numWidth(num);
  u8g2.setCursor(x + ((w/2) - (nw/2)), y + (size*0.55));
  u8g2.print(num);
}

void drawD100(int x, int y, int size, int num) {
  int dec = floor(num/10);
  int nom = num % 10;
  drawD10(x, y, size, dec, true);
  drawD10(x+(size*(0.25)+(x*0.5)), y + (size*0.3), size, nom, false);
}

void drawD10(int x, int y, int size, int num, bool hundo) {
  //draw a 10 sided shape around
  int side = size * 0.32495;
  int w = size*0.9;
  int x0 = x + w*0.5;
  int y0 = y;
  int x1 = x + w;
  int y1 = y + size*0.4;
  int x2 = x1;
  int y2 = y + size*0.6;
  int x3 = x0;
  int y3 = y + size;
  int x4 = x;
  int y4 = y2;
  int x5 = x4;
  int y5 = y1;


  u8g2.setDrawColor(0);
  u8g2.drawTriangle(x0,y0, x5,y5, x1,y1);
  u8g2.drawBox(x5, y5, x1-x5, y4-y5);
  u8g2.drawTriangle(x2,y2, x3,y3, x4,y4);
  u8g2.setDrawColor(1);

  u8g2.drawLine(x0,y0,x1,y1);
  u8g2.drawLine(x1,y1,x2,y2);
  u8g2.drawLine(x2,y2,x3,y3);
  u8g2.drawLine(x3,y3,x4,y4);
  u8g2.drawLine(x4,y4,x5,y5);
  u8g2.drawLine(x5,y5,x0,y0);

  int x6 = x + w*0.2;
  int y6 = y + size*0.5;
  int x7 = x + w*0.8;
  int y7 = y6;
  int x8 = x0;
  int y8 = y + size*0.7;


  u8g2.drawLine(x0,y0,x6,y6);
  u8g2.drawLine(x0,y0,x7,y7);
  u8g2.drawLine(x6,y6,x8,y8);
  u8g2.drawLine(x7,y7,x8,y8);
  u8g2.drawLine(x6,y6,x4,y4);
  u8g2.drawLine(x8,y8,x3,y3);
  u8g2.drawLine(x7,y7,x2,y2);

  //draw number
  int nw = numWidth(num);
  if (hundo) {
    nw += numWidth(0);
  }
  u8g2.setCursor(x + ((w/2) - (nw/2)), y + (size*0.45));
  u8g2.print(num);
  if (hundo) {
    u8g2.print(0);
  }
}

void drawD20(int x, int y, int size, int num) {

  int w = size*0.9;
  //draw number
  int nw = numWidth(num);

  u8g2.setCursor(x + ((w/2) - (nw/2)), 2 + (w*0.6));
  u8g2.print(num);

  //draw 6 sides
  int x0 = x + w/2;
  int y0 = y;
  int x1 = x + w;
  int y1 = y + w*0.33;
  int x2 = x + w;
  int y2 = y + w*0.9;
  int x3 = x0;
  int y3 = y + size - 1;
  int x4 = x;
  int y4 = y2;
  int x5 = x;
  int y5 = y1;
  u8g2.drawLine(x0,y0,x1,y1);
  u8g2.drawLine(x1,y1,x2,y2);
  u8g2.drawLine(x2,y2,x3,y3);
  u8g2.drawLine(x3,y3,x4,y4);
  u8g2.drawLine(x4,y4,x5,y5);
  u8g2.drawLine(x5,y5,x0,y0);

  //draw triangle
  int tw = w * 0.7;
  float height = 0.866 * tw;
  int tx = x + (w/2) - (tw/2);
  int ty = y + (size/2) - (height*0.6);
  int tx0 = tx;
  int ty0 = ty + height;
  int tx1 = tx + (tw / 2);
  int ty1 = ty;
  int tx2 = tx + tw;
  int ty2 = ty + height;
  u8g2.drawLine(tx0,ty0,tx1,ty1);
  u8g2.drawLine(tx1,ty1,tx2,ty2);
  u8g2.drawLine(tx2,ty2,tx0,ty0);

  //draw connectors
  u8g2.drawLine(x0,y0,tx1,ty1);
  u8g2.drawLine(x1,y1,tx1,ty1);
  u8g2.drawLine(x1,y1,tx2,ty2);
  u8g2.drawLine(x2,y2,tx2,ty2);
  u8g2.drawLine(x3,y3,tx2,ty2);
  u8g2.drawLine(x3,y3,tx0,ty0);
  u8g2.drawLine(x4,y4,tx0,ty0);
  u8g2.drawLine(x5,y5,tx0,ty0);
  u8g2.drawLine(x5,y5,tx1,ty1);

}

void drawD8(int x, int y, int size, int num) {
  //int w = size*0.866;
  int w = size*0.9;
  drawTriLine(x,y,w);
  //Draw a 6 sided shape - TODO: remove re-use
  int x0 = x + w/2;
  int y0 = y;
  int x1 = x + w*0.9;
  int y1 = y + w*0.33;
  int x2 = x + w;
  int y2 = y + w*0.866;
  int x3 = x0;
  int y3 = y + size - 1;
  int x4 = x;
  int y4 = y2;
  int x5 = x + w * 0.1;
  int y5 = y1;
  u8g2.drawLine(x0,y0,x1,y1);
  u8g2.drawLine(x1,y1,x2,y2);
  u8g2.drawLine(x2,y2,x3,y3);
  u8g2.drawLine(x3,y3,x4,y4);
  u8g2.drawLine(x4,y4,x5,y5);
  u8g2.drawLine(x5,y5,x0,y0);
  //draw number
  int nw = numWidth(num);

  u8g2.setCursor(x + ((w/2) - (nw/2)), 1 + (w*0.6));
  u8g2.print(num);
}

int numWidth(int num) {
  //todo if it starts with 1 add a bit
  char cstr[3];
  itoa(num, cstr, 10);
  int width = u8g2.getStrWidth(cstr);
  if (cstr[0] == '1') {
    width = width + u8g2.getStrWidth(&cstr[0]) * 0.1;
    if (cstr[1] == '\0') {
      width = width + u8g2.getStrWidth(&cstr[0]) * 0.4;
    }
  }

  int w = 0;
  for(int i = 0; cstr[i] != '\0'; i++) {
    if (cstr[i] == '1') {
      w += u8g2.getStrWidth(&cstr[i]) * 1.5;
    } else {
      w += u8g2.getStrWidth(&cstr[i]);
    }
  }
//  switch (num) {
//    case 1:
//      width = width*1.5;
//      break;
//    case 11:
//      width = width*1.2;
//      break;
//  }
  return width;
}

void drawTriLine(int x, int y, int size) {
  float height = 0.866 * size;
  int x0 = x;
  int y0 = y + height;
  int x1 = x + (size / 2);
  int y1 = y;
  int x2 = x + size;
  int y2 = y + height;
  u8g2.drawLine(x0,y0,x1,y1);
  u8g2.drawLine(x1,y1,x2,y2);
  u8g2.drawLine(x2,y2,x0,y0);
}
