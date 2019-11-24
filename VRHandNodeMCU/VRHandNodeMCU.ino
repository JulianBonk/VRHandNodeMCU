/*
 Name:		VRHandNodeMCU.ino
 Created:	23.11.2019 19:00
 Author:	julia
*/

#include "ForceServo.h"
#include "EMS22A.h"
#include "A4988.h"
#include "SPI.h"

//Parameters for stepper
#define MOTOR_STEPS 2520
#define RPM 120
#define MOTOR_ACCEL 2000
#define MOTOR_DECEL 1000
#define DIR D2
#define STEP D1

//CSPins
int CSAtxPin = D4;
int CSEncoderPin = D3;

A4988 stepper(MOTOR_STEPS, DIR, STEP);

int degree = 20;

EMS22A Encoder(CSEncoderPin);

void setup() {
	stepper.begin(RPM);
	stepper.enable();
	stepper.setSpeedProfile(stepper.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

	SPI.begin();
	SPI.setFrequency(100000);
	Serial.begin(115200);
}

void loop() {
	//if nextAction() is 0 all pulses were sent
	unsigned wait_time = stepper.nextAction();
	if (stepper.nextAction()) {
		delay(10);
	}
	else {
		delay(10);
		//move by x degree
		stepper.startRotate(degree);
		degree = degree * -1;
	}

	Encoder.readPosition();
	Serial.println(Encoder.position);
	//Serial.printf("pos: %d   off: %d   inc: %d\n\n\n", Encoder.position, Encoder.positionOffset, Encoder.increment);
}