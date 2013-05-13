/*
* Si4735.h
*
* Created: 2012/12/01 20:28:29
*  Author: mkusunoki
*/


#ifndef SI4735_H_
#define SI4735_H_

#define SI4735ADR 0x11
#define FM 0
#define AM 1
#define LW 2
#define SW 3

class Radio {
	
	private:
	//char mode;
	int FMfreq;
	int FMrssi;
	int FMsni;
	int AMfreq;
	int AMrssi;
	int AMsni;
	char volume;
	
	public:
	Radio();
	void begin(char mode);
	void FMPowerOn();
	void AMPowerOn();
	void PowerDown();
	void setFMFreq(int freq);
	void setAMFreq(int freq);
	void seekUpFMFreq();
	void seekDownFMFreq();
	void seekUpAMFreq();
	void seekDownAMFreq();
	void updateFMTuneStatus();
	void updateAMTuneStatus();
	void updateFMRsqStatus();
	void updateAMRsqStatus();
	int getFMFreq();
	int getAMFreq();
	int getFMRssi();
	int getAMRssi();
	int getFMSni();
	int getAMSni();
	void setVolume(int vol);
	void upVolume();
	void downVolume();
	char getVolume();
	void unmute();
	void mute();
	void setMode(char mode);
	void setProperty(int prop, int val);
};



#endif /* SI4735_H_ */