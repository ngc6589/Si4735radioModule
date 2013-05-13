/*
* Wire.h
*
* Created: 2012/11/30 16:13:39
*  Author: mkusunoki
*/


#ifndef WIRE_H_
#define WIRE_H_

#define BUFFER_LEN 32
#define I2C_READY 0
#define I2C_MRX   1
#define I2C_MTX   2
#define I2C_SRX   3
#define I2C_STX   4

class Wire {
	
	private:
	char txBuf[BUFFER_LEN];
	char txBufIdx;
	char txBufLen;
	char txAddr;
	char rxBuf[BUFFER_LEN];
	char rxBufIdx;
	char rxBufLen;
	
	public:
	Wire();
	void begin();
	void beginTransmission(char address);
	void endTransmission();
	int write(char ch);
	int write(char *ch, int len);
	int requestFrom(char addr, char len);
	char read();
};



#endif /* WIRE_H_ */