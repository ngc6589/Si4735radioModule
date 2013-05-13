/*
* Si4735.cpp
*
* Created: 2012/12/01 20:28:49
*  Author: mkusunoki
*/

#include <avr/io.h>
#include <util/delay.h>
#include "Si4735.h"
#include "Wire.h"


extern Wire i2c;

Radio::Radio() {
	;
}

void Radio::begin(char mode) {
	
	switch(mode) {
		case FM:
		FMPowerOn();
		setProperty(0x1400, 0x1DB0);	// FM_SEEK_BAND_BOTTOM 76.0MHz
		setProperty(0x1401, 0x2328);	// FM_SEEK_BAND_TOP 90.0MHz
		setProperty(0x1100, 0x0001);	// FM_DEEMPHASIS
		break;
		
		case AM:
		AMPowerOn();
		setProperty(0x3400, 0x0213);	// AM_SEEK_BAND_BOTTOM 531kHz
		setProperty(0x3401, 0x0642);	// AM_SEEK_BAND_BOTTOM 1602kHz
		setProperty(0x3402, 0x0009);	// AM_SEEK_FREQ_SPACING 9kHz
	}

	// Set the RCLK to 32768Hz
	setProperty(0x0201, 0x8000);
}

void Radio::FMPowerOn() {

	i2c.beginTransmission(SI4735ADR);
	i2c.write(0x01);
	i2c.write(0x10);
	i2c.write(0x05);
	i2c.endTransmission();
	_delay_ms(500);
}

void Radio::AMPowerOn() {

	i2c.beginTransmission(SI4735ADR);
	i2c.write(0x01);
	i2c.write(0x11);
	i2c.write(0x05);
	i2c.endTransmission();
	_delay_ms(500);
}

void Radio::PowerDown() {

	i2c.beginTransmission(SI4735ADR);
	i2c.write(0x11);
	i2c.endTransmission();
	_delay_ms(2);
}

void Radio::setFMFreq(int freq) {
	char hbyte, lbyte;
	
	hbyte = freq>>8;
	lbyte = freq & 0x00FF;
	i2c.beginTransmission(SI4735ADR);
	i2c.write(0x20);
	i2c.write(0x00);
	i2c.write(hbyte);
	i2c.write(lbyte);
	i2c.endTransmission();
	_delay_ms(100);
}

void Radio::setAMFreq(int freq) {
	char hbyte, lbyte;
	
	hbyte = freq>>8;
	lbyte = freq & 0x00FF;
	i2c.beginTransmission(SI4735ADR);
	i2c.write(0x40);
	i2c.write(0x00);
	i2c.write(hbyte);
	i2c.write(lbyte);
	i2c.write(0x00);
	i2c.write(0x00);
	i2c.endTransmission();
	_delay_ms(100);
}

void Radio::seekUpFMFreq() {

	i2c.beginTransmission(SI4735ADR);
	i2c.write(0x21);
	i2c.write(0x0C);
	i2c.endTransmission();
	_delay_ms(100);
}

void Radio::seekDownFMFreq() {

	i2c.beginTransmission(SI4735ADR);
	i2c.write(0x21);
	i2c.write(0x04);
	i2c.endTransmission();
	_delay_ms(100);
}

void Radio::seekUpAMFreq() {

	i2c.beginTransmission(SI4735ADR);
	i2c.write(0x41);
	i2c.write(0x0C);
	i2c.write(0x00);
	i2c.write(0x00);
	i2c.endTransmission();
	_delay_ms(100);
}

void Radio::seekDownAMFreq() {

	i2c.beginTransmission(SI4735ADR);
	i2c.write(0x41);
	i2c.write(0x04);
	i2c.write(0x00);
	i2c.write(0x00);
	i2c.endTransmission();
	_delay_ms(100);
}


void Radio::updateFMTuneStatus() {
	int i;
	
	i2c.beginTransmission(SI4735ADR);
	i2c.write(0x22);
	i2c.write(0x20);
	i2c.endTransmission();
	_delay_ms(2);

	i2c.requestFrom(SI4735ADR, 6);
	for(int i = 0; i < 6; i++) {
		switch(i) {
			case 2:
			FMfreq = i2c.read() * 256;
			break;
			case 3:
			FMfreq += i2c.read();
			break;
			case 4:
			FMrssi = i2c.read();
			break;
			default:
			i2c.read();
		}
	}
}

void Radio::updateFMRsqStatus() {
	int i;
	
	i2c.beginTransmission(SI4735ADR);
	i2c.write(0x23);
	i2c.write(0x20);
	i2c.endTransmission();
	_delay_ms(2);

	i2c.requestFrom(SI4735ADR, 6);
	for(int i = 0; i < 6; i++) {
		switch(i) {
			case 4:
			FMrssi = i2c.read();
			break;
			default:
			i2c.read();
		}
	}
}


void Radio::updateAMTuneStatus() {
	int i;
	
	i2c.beginTransmission(SI4735ADR);
	i2c.write(0x42);
	i2c.write(0x00);
	i2c.endTransmission();
	_delay_ms(2);

	i2c.requestFrom(SI4735ADR, 6);
	for(int i = 0; i < 6; i++) {
		switch(i) {
			case 2:
			AMfreq = i2c.read() * 256;
			break;
			case 3:
			AMfreq += i2c.read();
			break;
			case 4:
			AMrssi = i2c.read();
			break;
			default:
			i2c.read();
		}
	}
}

void Radio::updateAMRsqStatus() {
	int i;
	
	i2c.beginTransmission(SI4735ADR);
	i2c.write(0x43);
	i2c.write(0x00);
	i2c.endTransmission();
	_delay_ms(2);

	i2c.requestFrom(SI4735ADR, 6);
	for(int i = 0; i < 6; i++) {
		switch(i) {
			case 4:
			AMrssi = i2c.read();
			break;
			default:
			i2c.read();
		}
	}
}
int Radio::getFMFreq() {
	
	return(FMfreq);
}

int Radio::getAMFreq() {
	
	return(AMfreq);
}

int Radio::getFMRssi() {
	
	return(FMrssi);
}

int Radio::getAMRssi() {
	
	return(AMrssi);
}

int Radio::getFMSni() {
	
	return(FMsni);
}

int Radio::getAMSni() {
	
	return(AMsni);
}

void Radio::setVolume(int vol) {
	
	if(vol > 63 | vol == 0) {
		setProperty(0x4000, 63);
		volume = 63;
	} else {
		setProperty(0x4000, vol);
		volume = vol;
	}
		_delay_ms(2);
}

void Radio::upVolume() {
	
	if(volume < 63) {
		volume++;
		setVolume(volume);
	}
}

void Radio::downVolume() {
	
	if(volume > 2) {
		volume--;
		setVolume(volume);
	}
}

char Radio::getVolume() {
	return(volume);
}

void Radio::unmute() {
	
	setProperty(0x4001, 0x0000);

}

void Radio::mute() {
	
	setProperty(0x4001, 0x0003);
}

void Radio::setMode(char mode) {
	
		//mode = mode1;
		begin(mode);
}

void Radio::setProperty(int prop, int val) {
	char propertyHighByte;
	char propertyLowByte;
	char propertyValueHighByte;
	char propertyValueLowByte;
	
	propertyHighByte = prop>>8;
	propertyLowByte = prop & 0x00FF;
	propertyValueHighByte = val>>8;
	propertyValueLowByte = val & 0x00FF;
	
	i2c.beginTransmission(SI4735ADR);
	i2c.write(0x12);
	i2c.write(0x00);
	i2c.write(propertyHighByte);
	i2c.write(propertyLowByte);
	i2c.write(propertyValueHighByte);
	i2c.write(propertyValueLowByte);
	i2c.endTransmission();
	_delay_ms(20);
}