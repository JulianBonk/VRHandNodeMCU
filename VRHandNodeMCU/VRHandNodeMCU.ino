/*
 Name:		VRHandNodeMCU.ino
 Created:	23.11.2019 19:00
 Author:	julia
*/

#include "configuration.h" //configure parameters
#include "ATXCommunication.h" //custom communication library
#include "EMS22A.h"	//custom encoder library
#include "AccelStepper.h"
#include "SPI.h"
//test

#define DEBUG

ATXCommunication Atx(CSAtxPin, SPIFrequencyAtx);
EMS22A Encoder(CSEncoderPin, SPIFrequencyEncoder);
AccelStepper Stepper(1, D1, D2);

//processparameters
//Servo
int servoIntegral[2] = { 0, 0 };
int servoForceDelta[2] = { 0, 0 };
int servoForceTarget[2] = { servo1ForceFreerun, servo1ForceFreerun };
int servoPFactor[2] = { servo1PFactor,servo2PFactor };
int servoIFactor[2] = { servo1IFactor, servo2IFactor };
int servoForceFreerun[2] = { servo1ForceFreerun, servo2ForceFreerun };
int servoStiffness[2] = { servo1Stiffness, servo2Stiffness };
int servoFreerunPosition[2] = { servo1FreerunPosition, servo2FreerunPosition };
//BLDC
int BldcPositionTarget = 0;
int BldcPositionDelta = 0;
int BldcForceRaw = 0;
int BldcForce = 0;
int BldcForceOffset = 0;
int BldcForceTarget = confBldcForceFreerun;
int BldcForceDelta = 0;
int BldcIntegral = 0;
int BldcPFactor = confBldcPFactor;
int BldcIFactor = confBldcIFactor;
int BldcForceFreerun = confBldcForceFreerun;
int BldcStiffness = confBldcStiffness;
int BldcFreerunPosition = confBldcFreerunPosition;

//timing variables
unsigned long SpiMicroTime = 3000;
unsigned long SpiMicrosBefore;

#ifdef DEBUG
int debugMilliTime = 200;
int debugMillisBefore;
#endif // DEBUG

void setup() {
	////stepper setup
	Stepper.setMaxSpeed(bldcSpeed);
	Stepper.setAcceleration(bldcAcceleration);

	//SPI setup
	SPI.begin();
	SPI.setFrequency(1000000);

	//Serial setup only if DEBUG is defined
#ifdef DEBUG
	Serial.begin(115200);
	pinMode(D3, OUTPUT);
	pinMode(D3, HIGH);
#endif //DEBUG
	delay(5000);
}

void loop() {
	if ((micros() - SpiMicrosBefore) > SpiMicroTime) {
		SpiMicrosBefore = micros();
		Encoder.readPosition();
		Atx.current = 0xFF;
		Atx.sendAndReceiveAll();
	}

#ifdef DEBUG
	if ((millis() - debugMillisBefore) > debugMilliTime) {
		debugMillisBefore = millis();
		Serial.printf("Bldc: %d\tEncoder: %d\tForce: %d\n", Atx.BldcMagneticPosition, Encoder.position, BldcForce);
		//Serial.printf("Force: %d\tTarget: %d\n", BldcForce, BldcPositionTarget);
	}
#endif // DEBUG

	BldcForceRaw = Encoder.position - Atx.BldcMagneticPosition;
	if (BldcForce <= -73) {
		BldcForceOffset++;
	}
	if (BldcForce >= 73) {
		BldcForceOffset--;
	}
	BldcForce = 146 * BldcForceOffset + BldcForceRaw;

	//BldcForceDelta = BldcForceTarget - BldcForce;
	//BldcIntegral += BldcForceDelta;
	//BldcPositionTarget = BldcPFactor * BldcForceDelta + BldcIntegral * BldcIFactor;

	//Stepper.move(Stepper.currentPosition() + BldcPositionTarget);
	//Stepper.run();

	//for (int i = 0; i < 1; i++) {
	//	servoForceDelta[i] = servoForceTarget[i] - Atx.servoForce[i];
	//	servoIntegral[i] += servoForceDelta[i];
	//	Atx.servoPosition[i] = servoPFactor[i] * servoForceDelta[i] + servoIFactor[i] * servoIntegral[i];
	//	if (Atx.servoPosition[i] <= servoFreerunPosition[i]) {
	//		servoForceTarget[i] = servoForceFreerun[i];
	//	}
	//	else {
	//		servoForceTarget[i] = Atx.servoPosition[i] * servoStiffness[i] + servoForceFreerun[i];
	//	}
	//}
}