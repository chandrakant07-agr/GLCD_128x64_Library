#include <GLCD_KS0108.h>

GLCD_KS0108::GLCD_KS0108(uint8_t DI, uint8_t RW, uint8_t EN, 
		uint8_t D0, uint8_t D1, uint8_t D2, uint8_t D3, uint8_t D4, 
		uint8_t D5, uint8_t D6, uint8_t D7, uint8_t CS1, uint8_t CS2) {

	init(DI, RW, EN, D0, D1, D2, D3, D4, D5, D6, D7, CS1, CS2);
}

void GLCD_KS0108::init(uint8_t DI, uint8_t RW, uint8_t EN, 
		uint8_t D0, uint8_t D1, uint8_t D2, uint8_t D3, uint8_t D4, 
		uint8_t D5, uint8_t D6, uint8_t D7, uint8_t CS1, uint8_t CS2) {

    rsPin = DI;
    rwPin = RW;
    enPin = EN;
    c1Pin = CS1;
    c2Pin = CS2;

    dPins[0] = D0;
    dPins[1] = D1;
    dPins[2] = D2;
    dPins[3] = D3;
    dPins[4] = D4;
    dPins[5] = D5;
    dPins[6] = D6;
    dPins[7] = D7;
					
	dataPinsSize = sizeof(dPins)/sizeof(uint8_t);
	orientationStatus = Default_Top;
}

void GLCD_KS0108::begin(Colors color) {
    this->virtualCoord.x = 0;
	this->virtualCoord.y = 0;
	this->virtualCoord.p = 0;
	this->inverter = color;

    _pinDefine(rsPin, OUTPUT);
    _pinDefine(rwPin, OUTPUT);
    _pinDefine(enPin, OUTPUT);
    _pinDefine(c1Pin, OUTPUT);
    _pinDefine(c2Pin, OUTPUT);

    for(uint8_t i=0; i<dataPinsSize; i++) _pinDefine(dPins[i], OUTPUT);

    _delay_ms(DELAY_MS);				// wait 50ms (millisecond) for LCD to power up

    _pinWrite(rsPin, LOW);				// RS or D/I(Data/Instruction) Pin
	_pinWrite(rwPin, LOW);				// RW(Data Read/Data Write) Pin
	_pinWrite(enPin, LOW);				// Enable Signal(LOW = Off, HIGH = On)
	_pinWrite(c1Pin, HIGH);				// Select Chip1(LOW = Off, HIGH = On)
	_pinWrite(c2Pin, HIGH);				// Select Chip2(LOW = Off, HIGH = On)

    _ram_write_command(LCD_ON);			// LCD Contrast On
	_ram_write_command(LCD_START);		// Set Start Line

	clearDisplay(color ? Black : White);
	gotoXY(0, 0);
}

void GLCD_KS0108::gotoXY(uint8_t x, uint8_t y) {
	this->lcdCoord.x = x;
	this->lcdCoord.y = y;

	adjustOrientation(&x, &y);

    // save new coordinates
	this->virtualCoord.x = x;
	this->virtualCoord.y = y;
	this->virtualCoord.p = y/BIT_8;

	for(uint8_t i=0; i < DISPLAY_WIDTH/CHIP_WIDTH; i++) {
		_select_chip(i);
		_ram_write_command(LCD_PAGE | virtualCoord.p);
	}

	_select_chip(virtualCoord.x/CHIP_WIDTH);
	_ram_write_command(LCD_ADDR | virtualCoord.x % CHIP_WIDTH);
}

void GLCD_KS0108::clearDisplay(uint8_t color) {
	for(uint8_t i=0; i < BIT_8; i++) {
		for(uint8_t j=0; j < DISPLAY_WIDTH; j++) {
			gotoXY(j, i*BIT_8);
				_ram_write_data(color);
			__delay_ns();
		}
	}
}

void GLCD_KS0108::setDisplayColor(Colors color) {
	if(inverter != color) {
		inverter = color;
	}
}

void GLCD_KS0108::setPixel(uint8_t x, uint8_t y) {
	uint8_t pageOffset, rData, pixel = BIT_1;

	gotoXY(x, y);
	pageOffset = virtualCoord.y%BIT_8;
	pixel <<= pageOffset;
	rData = read_byte();
	pixel |= rData;
	_ram_write_data(pixel);
}

void GLCD_KS0108::printCharacter(uint8_t num) {
	char buffer[10];
	itoa(num, buffer, 10);
	printCharacter(buffer);
}

void GLCD_KS0108::printCharacter(const char* str) {
	while(*str != 0) draw_char(*(str++));
}

uint8_t GLCD_KS0108::read_byte(void) {
	_ram_read_byte();				// virtual read
	return _ram_read_byte();		// actual read
}

void GLCD_KS0108::drawHorizontalLine(uint8_t x, uint8_t y, uint8_t width) {
	for(uint8_t i=0; i<width; i++) setPixel(x+i, y);
}

void GLCD_KS0108::drawVerticallyLine(uint8_t x, uint8_t y, uint8_t height) {
	for(uint8_t i=0; i<height; i++) setPixel(x, y+i);
}

void GLCD_KS0108::drawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
	drawHorizontalLine(x, y, width-1);					// top
	drawVerticallyLine(x+width-1, y, height-1);			// right
	drawHorizontalLine(x+1, y+height-1, width-1);		// bottom
	drawVerticallyLine(x, y+1, height-1);				// left
}

void GLCD_KS0108::drawRoundedRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
		uint8_t r1) {

	drawRoundedRect(x, y, width, height, r1, r1, r1, r1);
}

void GLCD_KS0108::drawRoundedRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
		uint8_t r1, uint8_t r2) {

	drawRoundedRect(x, y, width, height, r1, r2, r1, r1);
}

void GLCD_KS0108::drawRoundedRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
		uint8_t r1, uint8_t r2, uint8_t r3) {

	drawRoundedRect(x, y, width, height, r1, r2, r3, r1);
}

void GLCD_KS0108::drawRoundedRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
		uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4) {

	r1 = (r1>width/2||r1>height/2)?((width>height)?height/2:width/2):r1;
	r2 = (r2>width/2||r2>height/2)?((width>height)?height/2:width/2):r2;
	r3 = (r3>width/2||r3>height/2)?((width>height)?height/2:width/2):r3;
	r4 = (r4>width/2||r4>height/2)?((width>height)?height/2:width/2):r4;

	draw_corner(x, y, width, height, r1, BIT_1);			// top-left
	draw_corner(x, y, width, height, r2, BIT_2);			// top-right
	draw_corner(x, y, width, height, r3, BIT_4);			// bottom-right
	draw_corner(x, y, width, height, r4, BIT_8);			// bottom-left

	drawHorizontalLine(x+r1, y, width-(r1+r2));				// top
	drawVerticallyLine(x+width-1, y+r2, height-(r2+r3));	// right
	drawHorizontalLine(x+r4, y+height-1, width-(r3+r4));	// bottom
	drawVerticallyLine(x, y+r1, height-(r4+r1));			// left
}

void GLCD_KS0108::drawCircle(uint8_t x, uint8_t y, uint8_t radius) {
	uint8_t diameter = 2*radius;
	drawRoundedRect(x, y, diameter, diameter, radius);
}

GLCD_KS0108& GLCD_KS0108::setOrientation(Orientation mode) {
	orientationStatus = mode;
	return *this;
}

void GLCD_KS0108::selectFontFamily(Font_t fontType) {
	Font_Family = fontType;
}


// support members for display draw 
uint8_t GLCD_KS0108::bitsSwaper(uint8_t byte) {
	byte = ((byte & 0xF0)>>4) | ((byte & 0x0F)<<4);
	byte = ((byte & 0xCC)>>2) | ((byte & 0x33)<<2);
	byte = ((byte & 0xAA)>>1) | ((byte & 0x55)<<1);

	return byte;
}

void GLCD_KS0108::fontMeasurement(uint8_t length, uint8_t width) {
	Font_Length = pgm_read_byte(&Font_Family[length]);
	Font_Width = pgm_read_byte(&Font_Family[width]);
}

void GLCD_KS0108::adjustOrientation(uint8_t* x, uint8_t* y) {
	uint8_t temp = *x;

	if(orientationStatus & BIT_1) {
		fontMeasurement(BIT_0, BIT_0);
	} else if(orientationStatus & BIT_2) {
		fontMeasurement(BIT_1, BIT_0);
		*x = (DISPLAY_WIDTH-BIT_1)-*y;
		*y = temp;
	} else if(orientationStatus & BIT_4) {
		fontMeasurement(BIT_0, BIT_0);
		*x = (DISPLAY_WIDTH-BIT_1)-*x;
		*y = (DISPLAY_HEIGHT-BIT_1)-*y;
	} else if(orientationStatus & BIT_8) {
		fontMeasurement(BIT_1, BIT_0);
		*x = *y;
		*y = (DISPLAY_HEIGHT-BIT_1)-temp;
	}
}


// display draw controls members
void GLCD_KS0108::draw_corner(uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
		uint8_t radius, uint8_t selector) {

	uint8_t x1 = 0, y1 = 0;
	int8_t frame = 1-radius, frameX = 1, frameY = -2*radius;

	while(frameY <= -4) {
		if(frame >= 0) {
			y1++;
			frameY += 2;
			frame += frameY-1;
		}

		if(frame <= 0) {
			x1++;
			frameX += 2;
			frame += frameX;
		}

		if(selector&BIT_1) setPixel(x+radius-x1, y+y1);						// top-left
		if(selector&BIT_2) setPixel(x+width-radius+x1-1, y+y1);				// top-right
		if(selector&BIT_4) setPixel(x+width-radius+x1-1, y+height-y1-1);	// bottom-right
		if(selector&BIT_8) setPixel(x+radius-x1, y+height-y1-1);			// bottom-left
	}
}

void GLCD_KS0108::draw_char(uint8_t _char) {
	#define F_SPACE (Font_Width+BIT_1)

	uint8_t pageOffset, rData, fontByte, newFontByte, bounds_width;
	uint16_t fontStart;

	bool is_vertical_direction = ((orientationStatus & BIT_2) || (orientationStatus & BIT_8));
	bool is_bottom_or_left_direction = ((orientationStatus & BIT_4) || (orientationStatus & BIT_8));

	// Remove leading empty characters
	// 32 is the ASCII of the first printable character
	fontStart = ((_char-BIT_32)*Font_Length)+((is_vertical_direction)?(BIT_95*Font_Width)+BIT_2:BIT_2);

	for(uint8_t index=0; index<Font_Length; index++) {
		// Find the start of the character in the font array
		// The first byte of each line is the width/height of the character
		fontByte = pgm_read_byte(&*(Font_Family + (fontStart+index)));

		if(orientationStatus & BIT_2)
			fontByte = bitsSwaper(fontByte)>>BIT_3;
		else if(orientationStatus & BIT_4)
			fontByte = bitsSwaper(fontByte);
		else if(orientationStatus & BIT_8)
			fontByte = fontByte<<BIT_3;

		// If the character exceeds screen width bounds, start the next line
		bounds_width = (is_vertical_direction) ? DISPLAY_HEIGHT : DISPLAY_WIDTH;
		if(lcdCoord.x >= (bounds_width-Font_Width) && index == BIT_0) gotoXY(0, lcdCoord.y+BIT_8);

		// Read the data from where we will start writing the font
		rData = read_byte();

		// set pageoffset
		pageOffset = (is_vertical_direction) ? lcdCoord.x%BIT_8 : lcdCoord.y%BIT_8;

		// Calculate overflowing bits related to page height
		if(pageOffset != BIT_0) {
			newFontByte = (is_bottom_or_left_direction) ? fontByte>>pageOffset : fontByte<<pageOffset;

			_ram_write_data(inverter
				? is_vertical_direction
					? (orientationStatus & BIT_8)
						? ~((newFontByte|BIT_FF<<BIT_8-pageOffset)
							| (newFontByte|BIT_FF>>F_SPACE+pageOffset))|rData
						: ~((newFontByte|BIT_FF>>BIT_8-pageOffset)
							| (newFontByte|BIT_FF<<F_SPACE+pageOffset))|rData
					: ~(newFontByte|BIT_FF>>BIT_8-pageOffset)|rData
				: newFontByte|rData);

			(is_vertical_direction)
				? gotoXY(lcdCoord.x + (BIT_8-pageOffset), lcdCoord.y)
				: gotoXY(lcdCoord.x, lcdCoord.y+BIT_8);

			// read the data from next page
			rData = read_byte();

			(is_bottom_or_left_direction)
				? fontByte<<=(BIT_8-pageOffset)
				: fontByte>>=(BIT_8-pageOffset);

			(is_vertical_direction)
				? lcdCoord.x-=(BIT_8-pageOffset)		// update the x-coordinate for the next line
				: lcdCoord.y-=BIT_8;					// update the y-coordinate for the next line
		}

		// If pageoffset does not overflow then write Bits otherwise write remaining overflown Bits
		_ram_write_data(inverter
			? is_vertical_direction
				? (orientationStatus & BIT_8)
		 			? ~(fontByte|BIT_FF>>(pageOffset?pageOffset-(BIT_8-F_SPACE):F_SPACE))|rData
		 			: ~(fontByte|BIT_FF<<(pageOffset?pageOffset-(BIT_8-F_SPACE):F_SPACE))|rData
				: ~(fontByte|BIT_FF<<(pageOffset?pageOffset:BIT_8))|rData
			: fontByte|rData);

		(is_vertical_direction)
			? lcdCoord.y++					// update the y-coordinate for the next font bits
			: lcdCoord.x++;					// update the x-coordinate for the next font bits

		gotoXY(lcdCoord.x, lcdCoord.y);
	}

	if(is_vertical_direction) {
		// update the x-coordinate for the next character
		// reset the y-coordinate for the next font bits
		gotoXY(lcdCoord.x+F_SPACE, lcdCoord.y-Font_Length);
	} else {
		// If inverter is BLACK then fill the space between two characters
		rData = read_byte();
		_ram_write_data((inverter<<pageOffset) | rData);

		if(pageOffset != BIT_0) {
			gotoXY(lcdCoord.x, lcdCoord.y+BIT_8);
			rData = read_byte();
			_ram_write_data((inverter>>(BIT_8-pageOffset)) | rData);
			gotoXY(lcdCoord.x, lcdCoord.y-BIT_8);
		}

		lcdCoord.x++;			// update the x-coordinate for the next character
	}
}


// LCD hardware controls members
void GLCD_KS0108::_select_chip(uint8_t chip) {
	if(chip == Chip_1)
		_pinWrite(c1Pin, HIGH);
	else
		_pinWrite(c1Pin, LOW);

	if(chip == Chip_2)
		_pinWrite(c2Pin, HIGH);
	else
		_pinWrite(c2Pin, LOW);
}

uint8_t GLCD_KS0108::_ram_read_byte(void) {
	uint8_t rData = 0;

	gotoXY(lcdCoord.x, lcdCoord.y);

	// set data pins in read mode
	for(uint8_t i=0; i<dataPinsSize; i++) _pinDefine(dPins[i], INPUT);

	_pinWrite(rsPin, HIGH);
	_pinWrite(rwPin, HIGH);

	_pinWrite(enPin, HIGH);
	__delay_ns();
	for(uint8_t i=0; i<dataPinsSize; i++) rData |= (_pinRead(dPins[i]) & BIT_1) << i;
	_pinWrite(enPin, LOW);
	__delay_ns();

	// reset data pins in write mode
	for(int i=0; i<dataPinsSize; i++) _pinDefine(dPins[i], OUTPUT);
	gotoXY(lcdCoord.x, lcdCoord.y);

	return rData;
}

void GLCD_KS0108::_ram_write_command(uint8_t _val) {
    __send_bits(_val, LOW);
}

void GLCD_KS0108::_ram_write_data(uint8_t _val) {
    __send_bits(_val, HIGH);
}

void GLCD_KS0108::__send_bits(uint8_t _val, uint8_t _mod) {
    _pinWrite(rsPin, _mod & BIT_1);
    _pinWrite(rwPin, _mod & BIT_0);

	__write_bits(_val);
}

void GLCD_KS0108::__write_bits(uint8_t _val) {
    for(uint8_t i=0; i<dataPinsSize; i++) _pinWrite(dPins[i], (_val >> i) & BIT_1);
    __pulse_enable();
}

// generating clock pulse, the brief delay in program execution using Assembly NOP instruction
void GLCD_KS0108::__pulse_enable(void) {
    _pinWrite(enPin, HIGH);
    __delay_ns();
    _pinWrite(enPin, LOW);
    __delay_ns();
}

void GLCD_KS0108::__delay_ns(void) {
    asm volatile("nop\n\t" "nop\n\t" "nop\n\t":::);
}
