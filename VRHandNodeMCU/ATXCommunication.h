// ATXCommunication.h

#ifndef _ATXCOMMUNICATION_h
#define _ATXCOMMUNICATION_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#endif

#include "SPI.h"

class ATXCommunication
{
public:
	ATXCommunication(int CsPin, int SpiFreq);
	~ATXCommunication();

	//functions
	void sendAndReceiveAll();
	void readAll();

	//variables
	int servoPosition[2];
	int servoForce[2];
	int BldcMagneticPosition;
	byte current;

private:
	//functions
	void m_combineHighAndLowByte();
	//variables
	int SpiFrequency;
	int m_CsPin;
	int m_iteration;
	int m_BldcPositionreceived;
	int m_BldcPositionBefore;
	int m_BldcMagneticPositionOffset;
	int m_ServoForceOffset[2];
	byte m_BldcPositionHB;
	byte m_BldcPositionLB;
	byte m_Servo1ForceHB;
	byte m_Servo1ForceLB;
	byte m_Servo2ForceHB;
	byte m_Servo2ForceLB;
	bool m_firstCall;
};