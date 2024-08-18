#include "Arduino.h"

#define BIT_0 0x00
#define BIT_1 0x01
#define BIT_7 0x07

// #include "Arduino/GLCD_Arduino.cpp"

void _pinDefine(uint8_t _p_, uint8_t _mod_);
void _pinWrite(uint8_t _p_, uint8_t _mod_);
uint8_t _pinRead(uint8_t _p_);

class GLCD_KS0108 {
    private:
        uint8_t dataPins[8];
    public:
        GLCD_KS0108(uint8_t DI, uint8_t RW, uint8_t EN, uint8_t D0, uint8_t D1, uint8_t D2, 
        uint8_t D3, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7, uint8_t CS1, uint8_t CS2);
        // ~GLCD_KS0108();
};

