/*
* Wire.cpp
*
* Created: 2012/11/30 16:13:58
*  Author: mkusunoki
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include "Wire.h"

Wire::Wire() {
	;
}

void Wire::begin(void) {

	PORTC |= (1<<PORTC5) | (1<<PORTC4);
	TWSR &= ~((1<<TWPS0) | (1<<TWPS1));
	// TWBR = (8MHz / 100kHz) - 16 / 2 = 32
	//TWBR = 32;
	// TWBR = (1MHz / 100kHz) - 16 / 2 = -3
	TWBR = 0;
	TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWEA);
}

void Wire::beginTransmission(char address) {
	txAddr = address;
	txBufIdx = 0;
	txBufLen = 0;
}

void Wire::endTransmission() {
	char i;

	// START Condition
	// TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE) | (1<<TWSTA);
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	loop_until_bit_is_set(TWCR, TWINT);
	
	// Check START ACK
	;
	
	// Send address
	TWDR = TW_WRITE;
	TWDR |= txAddr<<1;
	// TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE);
	TWCR = (1<<TWINT) | (1<<TWEN);
	loop_until_bit_is_set(TWCR, TWINT);
	
	// Check MT_SLA_ACK
	;
	
	// Send Data
	for(i = 0; i < txBufLen; i++) {

		TWDR = txBuf[i];
		// TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
		TWCR = (1<<TWINT) | (1<<TWEN);
		loop_until_bit_is_set(TWCR, TWINT);
		
		// Check MT_DATA_ACK
		;
	}
	
	// STOP Condition
	// TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTO);
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
	loop_until_bit_is_clear(TWCR, TWSTO);
}

int Wire::write(char ch) {

	if(txBufIdx < BUFFER_LEN) {
		txBuf[txBufIdx++] = ch;
		txBufLen = txBufIdx;
	} else {
		return(-1);
	}
	return(0);
}

int Wire::write(char *ch, int len) {
	char i;
	
	for(i = 0; i < len; i++) {
		write(ch[i]);
	}
	return(i);
}

int Wire::requestFrom(char addr, char len) {
	int i;

	rxBufIdx = 0;
	rxBufLen = 0;
	
	// START Condition
	// TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE) | (1<<TWSTA);
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	loop_until_bit_is_set(TWCR, TWINT);
	
	// Check START ACK
	;
	
	// Send address
	TWDR = TW_READ;
	TWDR |= txAddr<<1;
	// TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE);
	TWCR = (1<<TWINT) | (1<<TWEN);
	loop_until_bit_is_set(TWCR, TWINT);

	// READ DATA
	
	for(i = 0; i < len; i++) {
		if(i == (len - 1)) {
			TWCR = (1<<TWINT) | (1<<TWEN);  //NACK
		} else {
			TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);  //ACK
		}
		loop_until_bit_is_set(TWCR, TWINT);
		rxBuf[rxBufLen] = TWDR;
		rxBufLen++;
	}

	// STOP Condition
	// TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTO);
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
	loop_until_bit_is_clear(TWCR, TWSTO);
}

char Wire::read() {
	
	if(rxBufIdx < rxBufLen) {
		return(rxBuf[rxBufIdx++]);
	} else {
		return(0);
	}
}

SIGNAL(TWI_vect) {
	;
}