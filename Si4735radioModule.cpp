/*
 * Si4735radioModule.cpp
 *
 * Created: 2013/05/13 13:39:45
 *  Author: mkusunoki
 */ 



#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "Wire.h"
#include "lcd.h"
#include "Si4735.h"
#include "button.h"

void setup();
void loop();

// -----------------------------------------------------------------------------
// Constractor
// -----------------------------------------------------------------------------
Wire i2c;
Lcd charLcd;
Radio radio;
BUTTON btnPwrOff(0, PORTD3);
BUTTON btnMode(0, PORTD5);
BUTTON btnDown(0, PORTD6);
BUTTON btnUp(0, PORTD7);
BUTTON btnVolDown(1, PORTB0);
BUTTON btnVolUp(1,PORTB1);
BUTTON btnLcd(1, PORTB2);

// -----------------------------------------------------------------------------
// EEPROM
// -----------------------------------------------------------------------------
uint16_t EEMEM E_ffreq = 8510;
uint16_t EEMEM E_afreq = 666;
uint8_t EEMEM E_vol = 47;
uint8_t EEMEM E_mode = 0;
uint8_t EEMEM E_contrastIdx = 3;

// -----------------------------------------------------------------------------
// Grobal Variables
// -----------------------------------------------------------------------------
int ffreq = 8020;
int afreq = 828;
char vol = 46;
char mode = 0;
char contrastIdx = 3;
char contrastVal[6] = {
	0x2c,
	0x30,
	0x034,
	0x038,
	0x3C,
	0x3F
};

// -----------------------------------------------------------------------------
// Resume PowerDownMode (Dummy)
// -----------------------------------------------------------------------------
ISR (INT0_vect) {
	;
}

// -----------------------------------------------------------------------------
// Initial setup
// -----------------------------------------------------------------------------
void setup() {

	// Pullup Enable;
	MCUCR &= ~(1<<PUD);

	// PC1: LCD Power and I2C Bus Power
	// PC2: LCD Reset
	// PC3: Radio Reset
	DDRC |= (1<<PORTC3) | (1<<PORTC2) | (1<<PORTC1);
	PORTC |= (1<<PORTC1) | (1<<PORTC3) | (1<<PORTC2);

	// PD5: AM/FM Mode switch
	// PD6: Frequency down switch
	// PD7: Frequency up switch
	DDRD &= ~((1<<PORTD7) | (1<<PORTD6) | (1<<PORTD5));
	PORTD |= (1<<PORTD7) | (1<<PORTD6) | (1<<PORTD5);

	// PD2: INT0
	// PD3: Power off switch
	DDRD &= ~((1<<PORTD2) | (1<<PORTD3));
	PORTD |= (1<<PORTD2) | (1<<PORTD3);

	// PB0: Volume down switch
	// PB1: Volume up switch
	// PB2: Lcd contrast switch
	DDRB &= ~((1<<PORTB2) | (1<<PORTB1) | (1<<PORTB0));
	PORTB |= (1<<PORTB2) |(1<<PORTB1) | (1<<PORTB0);

	// Toggle radio module reset pin
	PORTC &= ~(1<<PORTC3);
	_delay_ms(5);
	PORTC |= (1<<PORTC3);
	_delay_ms(5);

	// Toggle lcd module reset pin
	PORTC &= ~(1<<PORTC2);
	_delay_ms(5);
	PORTC |= (1<<PORTC2);

	eeprom_busy_wait();
	ffreq = eeprom_read_word(&E_ffreq);
	eeprom_busy_wait();
	afreq = eeprom_read_word(&E_afreq);
	eeprom_busy_wait();
	vol = eeprom_read_byte(&E_vol);
	eeprom_busy_wait();
	mode = eeprom_read_byte(&E_mode);
	eeprom_busy_wait();
	contrastIdx = eeprom_read_byte(&E_contrastIdx);
	
	i2c.begin();
	charLcd.begin(contrastVal[contrastIdx]);
	charLcd.cls();

	radio.begin(mode);
	radio.mute();
	if(mode == 0) {
		radio.setFMFreq(ffreq);
	} else if(mode == 1) {
		radio.setAMFreq(afreq);
	}
	radio.setVolume(vol);
	_delay_ms(500);
	radio.unmute();
}

// -----------------------------------------------------------------------------
// main loop
// -----------------------------------------------------------------------------
void loop() {
	char msg[17];
	
	switch(mode) {
		case FM:
		radio.updateFMTuneStatus();
		radio.updateFMRsqStatus();
		ffreq = radio.getFMFreq();
		int tmp01, tmp02;
		tmp01 = radio.getFMFreq() / 100;
		tmp02 = (radio.getFMFreq() - (tmp01 * 100)) / 10;
		sprintf(msg, "FM %2d.%1dMHz S:%i", tmp01, tmp02, radio.getFMRssi());
		break;
		case AM:
		radio.updateAMTuneStatus();
		radio.updateAMRsqStatus();
		afreq = radio.getAMFreq();
		sprintf(msg, "AM %4dkHz S:%i", radio.getAMFreq(), radio.getAMRssi());
		break;
	}
	
	charLcd.setCursor1();
	charLcd.write(msg);
	sprintf(msg, "Vol:%2d", radio.getVolume());
	charLcd.setCursor2();
	charLcd.write(msg);
	
	_delay_ms(100);

	btnDown.update();
	switch(mode) {
		case FM:
		if(btnDown.isPress()) {
			radio.seekDownFMFreq();
		}
		case AM:
		if(btnDown.isPress()) {
			radio.seekDownAMFreq();
		}
	}
	
	btnUp.update();
	switch(mode) {
		case FM:
		if(btnUp.isPress()) {
			radio.seekUpFMFreq();
		}
		break;
		case AM:
		if(btnUp.isPress()) {
			radio.seekUpAMFreq();
		}
		break;
	}
	
	btnMode.update();
	if(btnMode.isPress()) {
		if(mode == 0) {
			radio.PowerDown();
			radio.begin(AM);
			radio.mute();
			radio.setAMFreq(afreq);
			radio.setVolume(vol);
			_delay_ms(500);
			radio.unmute();
			mode = 1;
		} else {

			radio.PowerDown();
			radio.begin(FM);
			radio.mute();
			radio.setFMFreq(ffreq);
			radio.setVolume(vol);
			_delay_ms(500);
			radio.unmute();
			mode = 0;
		}
	}
	
	btnVolUp.update();
	if(btnVolUp.isPress()) {
		radio.upVolume();
		vol = radio.getVolume();
	}
	
	btnVolDown.update();
	if(btnVolDown.isPress()) {
		radio.downVolume();
		vol = radio.getVolume();
	}
	
	btnLcd.update();
	if(btnLcd.isPress()) {
		contrastIdx++;
		if(contrastIdx > 5) {
			contrastIdx = 0;
		}
		charLcd.setContrast(contrastVal[contrastIdx]);
	}
	
	btnPwrOff.update();
	if(btnPwrOff.isPress()) {
		radio.PowerDown();
		PORTC = 0;
		DDRC = 0;
		PORTD = 0;
		DDRD = 0;
		PORTB = 0;
		DDRB = 0;
		
		eeprom_busy_wait();
		eeprom_write_word(&E_ffreq, ffreq);
		eeprom_busy_wait();
		eeprom_write_word(&E_afreq, afreq);
		eeprom_busy_wait();
		eeprom_write_byte(&E_vol, vol);
		eeprom_busy_wait();
		eeprom_write_byte(&E_mode, mode);
		eeprom_busy_wait();
		eeprom_write_byte(&E_contrastIdx, contrastIdx);
		
		_delay_ms(5);
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_enable();
		sei();
		sleep_cpu();
	}
}

// -----------------------------------------------------------------------------
// main
// -----------------------------------------------------------------------------
int main(void) {
	
	setup();
	while(1) {
		loop();
	}
}
