//
// enables the communication with the ATXMega board. For the protocol see the table in the cloud
//

#include "ATXCommunication.h"

ATXCommunication::ATXCommunication(int CsPin) {
	SPI.begin();
	m_CsPin = CsPin;
	pinMode(m_CsPin, OUTPUT);
	servoPosition[0] = 0;
	servoPosition[1] = 0;
	servoForce[0] = 0;
	servoForce[0] = 0;
	BldcMagneticPosition = 0;
	current = 0;
}

ATXCommunication::~ATXCommunication() {
}

void ATXCommunication::sendAndReceiveAll() {
	digitalWrite(m_CsPin, LOW);
	SPI.transfer(0x04);	//servo command and sets receive buffer to index 0
	m_setCsPinHighLow();
	m_BldcPositionHB = SPI.transfer((servoPosition[0] >> 8) & 0xFF);
	m_setCsPinHighLow();
	m_BldcPositionLB = SPI.transfer(servoPosition[0] & 0xFF);
	m_setCsPinHighLow();
	current = SPI.transfer((servoPosition[1] >> 8) & 0xFF);
	m_setCsPinHighLow();
	m_Servo1ForceHB = SPI.transfer(servoPosition[1] & 0xFF);
	m_setCsPinHighLow();
	m_Servo1ForceLB = SPI.transfer(0x02);
	m_setCsPinHighLow();
	m_Servo2ForceHB = SPI.transfer(current);
	m_setCsPinHighLow();
	m_Servo2ForceLB = SPI.transfer(0x00);

	m_combineHighAndLowByte();

	digitalWrite(m_CsPin, HIGH);
}

void ATXCommunication::readAll() {
	digitalWrite(m_CsPin, LOW);
	SPI.transfer(0x01);
	m_setCsPinHighLow();
	m_BldcPositionHB = SPI.transfer(0x00);
	m_setCsPinHighLow();
	m_BldcPositionLB = SPI.transfer(0x00);
	m_setCsPinHighLow();
	m_Servo1ForceHB = SPI.transfer(0x00);
	m_setCsPinHighLow();
	m_Servo1ForceLB = SPI.transfer(0x00);
	m_setCsPinHighLow();
	m_Servo2ForceHB = SPI.transfer(0x00);
	m_setCsPinHighLow();
	m_Servo2ForceLB = SPI.transfer(0x00);
	m_setCsPinHighLow();
	current = SPI.transfer(0x00);

	m_combineHighAndLowByte();

	digitalWrite(m_CsPin, HIGH);
}

void ATXCommunication::m_combineHighAndLowByte() {
	BldcMagneticPosition = m_BldcPositionHB << 8 + m_BldcPositionLB;
	servoForce[0] = m_Servo1ForceLB << 8 + m_Servo1ForceHB;
	servoForce[1] = m_Servo2ForceLB << 8 + m_Servo2ForceHB;
}

void ATXCommunication::m_setCsPinHighLow() {
	digitalWrite(m_CsPin, HIGH);
	digitalWrite(m_CsPin, LOW);
}