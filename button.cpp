/*
* button.cpp
*
* Created: 2012/12/03 13:44:07
*  Author: mkusunoki
*/

#include <avr/io.h>
#include "button.h"

BUTTON::BUTTON(int port, int pin) {
	btnPin = pin;
	btnPort = port;
	press = 0;
	btnStatus = 1;
	
	//pinMode(pin, INPUT_PULLUP);
}

void BUTTON::update() {
	int status;

	if(btnPort == 0) {
		if(bit_is_clear(PIND, btnPin) && btnStatus == 1) {
			btnStatus = 0;
			press = 1;
		}
		else if(bit_is_set(PIND, btnPin) && btnStatus == 0) {
			btnStatus = 1;
		}
	}
	if(btnPort == 1) {
		if(bit_is_clear(PINB, btnPin) && btnStatus == 1) {
			btnStatus = 0;
			press = 1;
		}
		else if(bit_is_set(PINB, btnPin) && btnStatus == 0) {
			btnStatus = 1;
		}
	}
}

int BUTTON::isPress() {
	if(press == 1) {
		press = 0;
		return(1);
	}
	else {
		return(0);
	}
}
