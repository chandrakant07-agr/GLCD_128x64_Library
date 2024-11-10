#pragma once

#include <inttypes.h>
#include "Arduino.h"

#define DISPLAY_WIDTH	128
#define DISPLAY_HEIGHT	64
#define CHIP_WIDTH      64

#define DELAY_MS 50

#define LCD_ON		0x3F
#define LCD_OFF		0x3E
#define LCD_START	0xC0
#define LCD_ADDR	0x40
#define LCD_PAGE	0xB8

#define BIT_0   0x00
#define BIT_1   0x01
#define BIT_2   0x02
#define BIT_3   0x03
#define BIT_4   0x04
#define BIT_5   0x05
#define BIT_6   0x06
#define BIT_7   0x07
#define BIT_8   0x08
#define BIT_15  0x0F
#define BIT_32  0x20
#define BIT_95  0x5F
#define BIT_FF  0xFF

struct Virtual_Coordinates {
    uint8_t x;
    uint8_t y;
    uint8_t p;
};

struct Coordinates {
    uint8_t x;
    uint8_t y;
};

enum Chip_Set {
    Chip_1, Chip_2
};

enum Colors {
    White = BIT_0,
    Black = BIT_FF
};

enum Orientation {
    Default_Top = BIT_1,
    Right_Top   = BIT_2,
    Bottom_Top  = BIT_4,
    Left_Top    = BIT_8
};

typedef const uint8_t* Font_t;

class GLCD_Arduino {
    private:
      /* data */
    public:
      // GLCD_Arduino(/* args */);
      void _pinDefine(uint8_t _pin, uint8_t _mod);
      void _pinWrite(uint8_t _pin, uint8_t _mod);
      uint8_t _pinRead(uint8_t _pin);
};

class GLCD_KS0108: public GLCD_Arduino {
    private:
        struct Virtual_Coordinates virtualCoord;
        uint8_t inverter;
        uint8_t dPins[8];
        uint8_t rsPin, rwPin, enPin, c1Pin, c2Pin;
        uint8_t dataPinsSize;
        uint8_t orientationStatus;
        uint8_t Font_Length;
        uint8_t Font_Width;
        Font_t Font_Family;

        // LCD hardware controls members
        void _select_chip(uint8_t chip);
        uint8_t _ram_read_byte(void);
        void _ram_write_command(uint8_t _val);
        void _ram_write_data(uint8_t _val);

        void __send_bits(uint8_t _val, uint8_t _mod);
        void __write_bits(uint8_t _val);
        void __pulse_enable(void);
        void __delay_ns(void);

        // support members for display draw 
        uint8_t bitsSwaper(uint8_t byte);
        void fontMeasurement(uint8_t lIndex, uint8_t wIndex);
        void adjustOrientation(uint8_t* x, uint8_t* y);

        // display draw controls members
        void draw_char(uint8_t _char);
        void draw_corner(uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
            uint8_t radius, uint8_t selector);

        // lcd pins initialize member
        void init(uint8_t DI, uint8_t RW, uint8_t EN, 
            uint8_t D0, uint8_t D1, uint8_t D2, uint8_t D3, uint8_t D4, 
            uint8_t D5, uint8_t D6, uint8_t D7, uint8_t CS1, uint8_t CS2);

    public:
        int debug = 0;
        struct Coordinates lcdCoord;
        // struct Virtual_Coordinates virtualCoord;

        // final user level members
        GLCD_KS0108(uint8_t DI, uint8_t RW, uint8_t EN, 
            uint8_t D0, uint8_t D1, uint8_t D2, uint8_t D3, uint8_t D4, 
            uint8_t D5, uint8_t D6, uint8_t D7, uint8_t CS1, uint8_t CS2);

        void begin(Colors color);
        void gotoXY(uint8_t x, uint8_t y);
        void clearDisplay(uint8_t color);
        void setDisplayColor(Colors color);
        void setPixel(uint8_t x, uint8_t y);
        void printCharacter(uint8_t num);
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

        GLCD_KS0108& setOrientation(Orientation mode);
        void selectFontFamily(Font_t fontType);

};
