/*
* lcd.cpp
*
* Created: 2012/12/01 19:52:47
*  Author: mkusunoki
*/

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "Wire.h"

extern Wire i2c;

Lcd::Lcd() {

	;
}

void Lcd::begin(char contrast) {

	_delay_ms(500);

	i2c.beginTransmission(LCD);
	i2c.write(0x00);
	i2c.write(0x38);
	i2c.endTransmission();
	_delay_ms(1);
	
	PORTC |= (1<<PORTC2);
	i2c.beginTransmission(LCD);
	i2c.write(0x00);
	i2c.write(0x39);
	i2c.endTransmission();
	_delay_ms(1);
	
	i2c.beginTransmission(LCD);
	i2c.write(0x00);
	i2c.write(0x14);
	i2c.endTransmission();
	_delay_ms(1);
	
	//setContrast(contrast);
	i2c.beginTransmission(LCD);
	i2c.write(0x00);
	i2c.write(0x70 | (contrast & 0xf));
	i2c.endTransmission();
	_delay_ms(1);
	
	i2c.beginTransmission(LCD);
	i2c.write(0x00);
	i2c.write(0x5C | ((contrast>>4) & 0x3));
	i2c.endTransmission();
	_delay_ms(1);
	
	i2c.beginTransmission(LCD);
	i2c.write(0x00);
	i2c.write(0x6C);
	i2c.endTransmission();
	_delay_ms(300);
	
	i2c.beginTransmission(LCD);
	i2c.write(0x00);
	i2c.write(0x38);
	i2c.endTransmission();
	_delay_ms(1);
	
	i2c.beginTransmission(LCD);
	i2c.write(0x00);
	i2c.write(0x0C);
	i2c.endTransmission();
	_delay_ms(1);
	
	i2c.beginTransmission(LCD);
	i2c.write(0x00);
	i2c.write(0x01);
	i2c.endTransmission();
	_delay_ms(2);
}

void Lcd::write(char ch) {
	i2c.beginTransmission(LCD);
	i2c.write(0x40);
	i2c.write(ch);
	i2c.endTransmission();
	_delay_ms(1);
}

void Lcd::write(char *str) {
	i2c.beginTransmission(LCD);
	i2c.write(0x40);
	i2c.write(str, strlen(str));
	i2c.endTransmission();
	_delay_ms(1);
	
}

void Lcd::setCursor2() {
	i2c.beginTransmission(LCD);
	i2c.write(0x00);
	i2c.write(0xC0);
	i2c.endTransmission();
	_delay_ms(1);
}

void Lcd::setCursor1() {
	i2c.beginTransmission(LCD);
	i2c.write(0x00);
	i2c.write(0x80);
	i2c.endTransmission();
	_delay_ms(1);
}

void Lcd::cls() {
	i2c.beginTransmission(LCD);
	i2c.write(0x00);
	i2c.write(0x01);
	i2c.endTransmission();
	_delay_ms(1);
}

void Lcd::setContrast(char contrast) {

	PORTC |= (1<<PORTC2);
	i2c.beginTransmission(LCD);
	i2c.write(0x00);
	i2c.write(0x39);
	i2c.endTransmission();
	_delay_ms(1);
	
	i2c.beginTransmission(LCD);
	i2c.write(0x00);
	i2c.write(0x70 | (contrast & 0xf));
	i2c.endTransmission();
	_delay_ms(1);
	
	i2c.beginTransmission(LCD);
	i2c.write(0x00);
	i2c.write(0x5C | ((contrast>>4) & 0x3));
	i2c.endTransmission();
	_delay_ms(1);
}