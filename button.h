/*
 * button.h
 *
 * Created: 2012/12/03 13:43:10
 *  Author: mkusunoki
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_


class BUTTON {

	private:
	int btnPin;
	int btnPort;
	int btnStatus;
	int press;

	public:
	BUTTON(int port, int pin);
	void update();
	int isPress();
};


#endif /* BUTTON_H_ */