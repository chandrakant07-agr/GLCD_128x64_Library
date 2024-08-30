#include <GLCD_KS0108.h>

#define PIN_7	7		// Arduino pin no. 7
#define PIN_8	8		// Arduino pin no. 8
#define PIN_13 	13		// Arduino pin no. 13
#define PIN_14 	14		// Arduino pin no. 14

void GLCD_Arduino::_pinDefine(uint8_t _pin, uint8_t _mod) {
	if(_mod == 0) {
		_pin<=PIN_7
			? DDRD&=~(BIT_1<<(_pin&BIT_7))
			: (_pin<=PIN_13
				? DDRB&=~(BIT_1<<((_pin-PIN_8)&BIT_7))
				: DDRC&=~(BIT_1<<((_pin-PIN_14)&BIT_7)));
	} else {
		_pin<=PIN_7
			? DDRD|=(1<<(_pin&BIT_7))
			: (_pin<=PIN_13
				? DDRB|=(BIT_1<<((_pin-PIN_8)&BIT_7))
				: DDRC|=(BIT_1<<((_pin-PIN_14)&BIT_7)));
	}
}

void GLCD_Arduino::_pinWrite(uint8_t _pin, uint8_t _mod) {
	if(_mod == 0) {
		_pin<=PIN_7
			? PORTD&=~(BIT_1<<(_pin&BIT_7))
			: (_pin<=PIN_13
				? PORTB&=~(BIT_1<<((_pin-PIN_8)&BIT_7))
				: PORTC&=~(BIT_1<<((_pin-PIN_14)&BIT_7)));
	} else {
		_pin<=PIN_7
			? PORTD|=(BIT_1<<(_pin&BIT_7))
			: (_pin<=PIN_13
				? PORTB|=(BIT_1<<((_pin-PIN_8)&BIT_7))
				: PORTC|=(BIT_1<<((_pin-PIN_14)&BIT_7)));
	}
}

uint8_t GLCD_Arduino::_pinRead(uint8_t _pin) {
	return (_pin<=PIN_7
				? PIND&(BIT_1<<(_pin&BIT_7))
				: (_pin<=PIN_13
					? PINB&(BIT_1<<((_pin-=PIN_8)&BIT_7))
					: PINC&(BIT_1<<((_pin-=PIN_14)&BIT_7)))
			)>>_pin;
}
