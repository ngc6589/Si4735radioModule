/*
* lcd.h
*
* Created: 2012/12/01 19:52:32
*  Author: mkusunoki
*/


#ifndef LCD_H_
#define LCD_H_

#define LCD 0x3e

class Lcd {
	
	private:

	public:
	Lcd();
	void begin(char contrast);
	void write(char ch);
	void write(char *str);
	void setCursor2();
	void setCursor1();
	void cls();
	void setContrast(char contrast);
};

#endif /* LCD_H_ */