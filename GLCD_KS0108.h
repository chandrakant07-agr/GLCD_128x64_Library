#pragma once

#include "Arduino.h"
#include "./fonts/FONT_8x5.h"

#define DISPLAY_WIDTH	128
#define DISPLAY_HEIGHT	64
#define CHIP_WIDTH		64

#define LCD_ON		0x3F
#define LCD_OFF		0x3E
#define LCD_START	0xC0
#define LCD_ADDR	0x40
#define LCD_PAGE	0xB8

#define BIT_0   0x00
#define BIT_1   0x01
#define BIT_2   0x02
#define BIT_4   0x04
#define BIT_7   0x07
#define BIT_8   0x08
#define BIT_15  0x0F
#define BIT_FF  0xFF

#define LOW_NIBBLE BIT_15

struct Coordinate {
  uint8_t x;
  uint8_t y;
  uint8_t p;
};

enum Chip_Set {
    Chip_1, Chip_2
};

enum Colors {
    White = BIT_0,
    Black = BIT_FF
};

class GLCD_Arduino {
    private:
      /* data */
    public:
      // GLCD_Arduino(/* args */);
      void _pinDefine(uint8_t _p_, uint8_t _mod_);
      void _pinWrite(uint8_t _p_, uint8_t _mod_);
      uint8_t _pinRead(uint8_t _p_);
};

class GLCD_KS0108: public GLCD_Arduino {
  private:
    //   const byte chipCondition[2] = {1, 2};
      uint8_t inverter;
      uint8_t dPins[8];
      uint8_t rsPin, rwPin, enPin, c1Pin, c2Pin;
      uint8_t dataPinsSize;
      // uint8_t displayBitMod;

      // low level methods
      void _select_chip(uint8_t chip);
      uint8_t _ram_read_byte(void);
      void _ram_write_command(uint8_t _val);
      void _ram_write_data(uint8_t _val);

      void __send_bits(uint8_t _val, uint8_t _mod);
      void __write_bits(uint8_t _val);
      void __pulse_enable(void);
      void __delay_ns(void);

      //mid level methods
      void draw_char(uint8_t _char);
      void draw_line(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
      void draw_corner(uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
          uint8_t radius, uint8_t selector);

      void init(uint8_t DI, uint8_t RW, uint8_t EN, 
          uint8_t D0, uint8_t D1, uint8_t D2, uint8_t D3, uint8_t D4, 
          uint8_t D5, uint8_t D6, uint8_t D7, uint8_t CS1, uint8_t CS2);

  public:
      int debug = 0;
      struct Coordinate lcdCoord;

      GLCD_KS0108(uint8_t DI, uint8_t RW, uint8_t EN, 
          uint8_t D0, uint8_t D1, uint8_t D2, uint8_t D3, uint8_t D4, 
          uint8_t D5, uint8_t D6, uint8_t D7, uint8_t CS1, uint8_t CS2);
      
      // GLCD_KS0108(uint8_t DI, uint8_t RW, uint8_t EN, uint8_t D4, 
      //     uint8_t D5, uint8_t D6, uint8_t D7, uint8_t CS1, uint8_t CS2);

      void begin(enum Colors color);
      void gotoXY(uint8_t x, uint8_t y);
      void clearDisplay(uint8_t color);
      void setDisplayColor(enum Colors color);
      void setPixel(uint8_t x, uint8_t y);
      void printNumber(uint8_t num);
      void printCharacter(const char* str);
      uint8_t read_byte(void);
      void drawHorizontalLine(uint8_t x, uint8_t y, uint8_t length);
      void drawVerticallyLine(uint8_t x, uint8_t y, uint8_t length);
      void drawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
      void drawCircle(uint8_t x, uint8_t y, uint8_t radius);

      void drawRoundedRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
          uint8_t radius1);

      void drawRoundedRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
          uint8_t radius1, uint8_t radius2);

      void drawRoundedRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
          uint8_t radius1, uint8_t radius2, uint8_t radius3);

      void drawRoundedRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
          uint8_t radius1, uint8_t radius2, uint8_t radius3, uint8_t radius4);

};
