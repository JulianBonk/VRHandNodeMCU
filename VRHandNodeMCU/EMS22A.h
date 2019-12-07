// EMS22A.h

#ifndef _EMS22A_h
#define _EMS22A_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#endif

#include "SPI.h"

class EMS22A
{
public:
	EMS22A() = delete;
	EMS22A(int EncoderCSPin, int SpiFreq);
	//functions
	void readPosition();
	void readAll();

	//variables
	int position;
	bool end;
	bool cordicError;
	bool linearityAlarm;
	bool magnitudeIncrease;
	bool magnitudeDecrease;
	bool parity;

private:
	//functions
	void readPositionOffset();

	//variables
	int SpiFrequency;
	int m_CSPin;
	int m_buffer;
	int m_bufferOld;
	int m_increment;
	int m_positionOffset;
	int m_received;
};