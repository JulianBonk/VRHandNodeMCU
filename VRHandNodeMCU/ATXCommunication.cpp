//
// enables the communication with the ATXMega board. For the protocol see the table in the cloud
//

#include "ATXCommunication.h"

ATXCommunication::ATXCommunication(int CsPin, int SpiFreq) {
	SPI.begin();
	SpiFrequency = SpiFreq;
	m_CsPin = CsPin;
	pinMode(m_CsPin, OUTPUT);
	digitalWrite(m_CsPin, HIGH);
	servoPosition[0] = 0;
	servoPosition[1] = 0;
	servoForce[0] = 0;
	servoForce[0] = 0;
	BldcMagneticPosition = 0;
	current = 0;
	m_iteration = 0;
	m_firstCall = true;
	m_BldcMagneticPositionOffset = 0;
	m_ServoForceOffset[0] = 0;
	m_ServoForceOffset[1] = 0;
}

ATXCommunication::~ATXCommunication() {
}

void ATXCommunication::sendAndReceiveAll() {
	//SPI.setFrequency(SpiFrequency);
	digitalWrite(m_CsPin, LOW);
	SPI.transfer(0x04);	//servo command and sets receive buffer to index 0
	m_BldcPositionHB = SPI.transfer((servoPosition[0] >> 8) & 0xFF);
	m_BldcPositionLB = SPI.transfer(servoPosition[0] & 0xFF);
	m_Servo1ForceHB = SPI.transfer((servoPosition[1] >> 8) & 0xFF);
	m_Servo1ForceLB = SPI.transfer(servoPosition[1] & 0xFF);
	m_Servo2ForceHB = SPI.transfer(0x02);
	m_Servo2ForceLB = SPI.transfer(current & 0xFF);
	SPI.transfer16(0x0000);
	SPI.transfer16(0x0000);
	current = SPI.transfer(0x00);

	m_combineHighAndLowByte();

	digitalWrite(m_CsPin, HIGH);
}

void ATXCommunication::readAll() {
	//SPI.setFrequency(SpiFrequency);
	digitalWrite(m_CsPin, LOW);
	SPI.transfer(0x01);
	m_BldcPositionHB = SPI.transfer(0x00);
	m_BldcPositionLB = SPI.transfer(0x00);
	m_Servo1ForceHB = SPI.transfer(0x00);
	m_Servo1ForceLB = SPI.transfer(0x00);
	m_Servo2ForceHB = SPI.transfer(0x00);
	m_Servo2ForceLB = SPI.transfer(0x00);
	SPI.transfer16(0x0000);
	SPI.transfer16(0x0000);
	current = SPI.transfer(0x00);

	m_combineHighAndLowByte();

	digitalWrite(m_CsPin, HIGH);
}

void ATXCommunication::m_combineHighAndLowByte() {
	m_BldcPositionreceived = m_BldcPositionHB * 256 + m_BldcPositionLB;
	servoForce[0] = m_Servo1ForceHB * 256 + m_Servo1ForceLB - m_ServoForceOffset[0];
	servoForce[1] = m_Servo2ForceHB * 256 + m_Servo2ForceLB - m_ServoForceOffset[0];

	if (m_BldcPositionBefore - m_BldcPositionreceived > 180) {
		m_iteration++;
	}
	if (m_BldcPositionBefore - m_BldcPositionreceived < -180) {
		m_iteration--;
	}

	if (m_firstCall) {
		m_BldcMagneticPositionOffset = m_BldcPositionreceived;
		m_ServoForceOffset[0] = servoForce[0];
		m_ServoForceOffset[1] = servoForce[1];
		m_firstCall = false;
	}

	m_BldcPositionBefore = m_BldcPositionreceived;
	BldcMagneticPosition = map((m_iteration * 360 + m_BldcPositionreceived - m_BldcMagneticPositionOffset), 0, 2520, 0, 1023);
}