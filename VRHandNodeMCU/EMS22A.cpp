#include "EMS22A.h"

//Constructor with one parameter
EMS22A::EMS22A(int EncoderCSPin)
{
	SPI.begin();
	buffer = 0;
	bufferOld = 0;
	increment = 0;
	positionOffset = 0;
	CSPin = EncoderCSPin;
	pinMode(CSPin, OUTPUT);
	readPositionOffset();
}

void EMS22A::readPosition() {
	bufferOld = buffer;
	digitalWrite(CSPin, LOW);
	buffer = SPI.transfer16(0) >> 6;
	digitalWrite(CSPin, HIGH);
	//checks for rotation
	if (buffer - bufferOld > 512) increment--;
	if (buffer - bufferOld < -512) increment++;

	position = (increment * 1023) + buffer - positionOffset;
}

void EMS22A::readPositionOffset() {
	digitalWrite(CSPin, LOW);
	positionOffset = SPI.transfer16(0) >> 6;
	digitalWrite(CSPin, HIGH);
}

void EMS22A::readAll() {
	digitalWrite(CSPin, LOW);
	received = SPI.transfer16(0);
	digitalWrite(CSPin, HIGH);

	position = received >> 6;
	end = received & (1 << 5);
	cordicError = received & (1 << 4);
	linearityAlarm = received & (1 << 3);
	magnitudeIncrease = received & (1 << 2);
	magnitudeDecrease = received & (1 << 1);
	parity = received & (1 << 0);
}