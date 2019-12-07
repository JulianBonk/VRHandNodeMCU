//
// enables easy communication with the encoder for highlevel access
//

#include "EMS22A.h"

//Constructor with one parameter
EMS22A::EMS22A(int EncoderCSPin, int SpiFreq)
{
	SPI.begin();
	SpiFrequency = SpiFreq;
	m_buffer = 0;
	m_bufferOld = 0;
	m_increment = 0;
	m_positionOffset = 0;
	m_CSPin = EncoderCSPin;
	pinMode(m_CSPin, OUTPUT);
	digitalWrite(m_CSPin, HIGH);
	readPositionOffset();
}

void EMS22A::readPosition() {
	//SPI.setFrequency(SpiFrequency);
	m_bufferOld = m_buffer;
	digitalWrite(m_CSPin, LOW);
	m_buffer = SPI.transfer16(0) >> 6;
	digitalWrite(m_CSPin, HIGH);
	//checks for rotation
	if (m_buffer - m_bufferOld > 512) m_increment--;
	if (m_buffer - m_bufferOld < -512) m_increment++;

	position = (m_increment * 1023) + m_buffer - m_positionOffset;
}

void EMS22A::readPositionOffset() {
	//SPI.setFrequency(SpiFrequency);
	digitalWrite(m_CSPin, LOW);
	m_positionOffset = SPI.transfer16(0) >> 6;
	digitalWrite(m_CSPin, HIGH);
	m_buffer = m_positionOffset;
}

void EMS22A::readAll() {
	//SPI.setFrequency(SpiFrequency);
	digitalWrite(m_CSPin, LOW);
	m_received = SPI.transfer16(0);
	digitalWrite(m_CSPin, HIGH);

	position = m_received >> 6;
	m_buffer = position;
	end = m_received & (1 << 5);
	cordicError = m_received & (1 << 4);
	linearityAlarm = m_received & (1 << 3);
	magnitudeIncrease = m_received & (1 << 2);
	magnitudeDecrease = m_received & (1 << 1);
	parity = m_received & (1 << 0);
}