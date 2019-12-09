/*
 Name:		VRHandNodeMCU.ino
 Created:	23.11.2019 19:00
 Author:	julia
*/

#include "configuration.h" //change parameters only here
#include "ATXCommunication.h" //custom communication library
#include "EMS22A.h"	//custom encoder library
#include "AccelStepper.h" //library for using the BLDC as stepper
#include "SPI.h" //default SPI library
//test

#define DEBUG //uncomment to disable serialprints and faster code execution

ATXCommunication Atx(CSAtxPin, SPIFrequencyAtx);  //TODO: SPI FREQUENCY CHECK
EMS22A Encoder(CSEncoderPin, SPIFrequencyEncoder);
AccelStepper Stepper(1, STEP, DIR);

//processparameters
//Servo
long longServoPosition[2];
int servoIntegral[2] = { 0, 0 };
int servoForceDelta[2] = { 0, 0 };
int servoForceTarget[2] = { servo1ForceFreerun, servo1ForceFreerun };
float servoPFactor[2] = { servo1PFactor,servo2PFactor };
float servoIFactor[2] = { servo1IFactor, servo2IFactor };
int servoForceFreerun[2] = { servo1ForceFreerun, servo2ForceFreerun };
float servoStiffness[2] = { servo1Stiffness, servo2Stiffness };
int servoFreerunPosition[2] = { servo1FreerunPosition, servo2FreerunPosition };
//BLDC
int BldcForceRaw = 0;
int BldcForce = 0;
int BldcForceOffset = 0;
int BldcForceTarget = confBldcForceFreerun;
int BldcForceDelta = 0;
int BldcIntegral = 0;
int BldcForceFreerun = confBldcForceFreerun;
int BldcFreerunPosition = confBldcFreerunPosition;
float BldcStiffness = confBldcStiffness;
float BldcPFactor = confBldcPFactor;
float BldcIFactor = confBldcIFactor;

//timing variables
unsigned long SpiMicroTime = 3000;
unsigned long SpiMicrosBefore;

//functions
void calculateBldcPosition();
void calculateServoPosition();

#ifdef DEBUG
int debugMilliTime = 500;
int debugMillisBefore;
#endif // DEBUG

void setup() {
	////stepper setup
	Stepper.setMaxSpeed(bldcSpeed);
	Stepper.setAcceleration(bldcAcceleration);
	Stepper.setCurrentPosition(0);
	Atx.current = bldcDefaultCurrent;

	//SPI setup
	SPI.begin();
	SPI.setFrequency(1000000);

	//Serial setup only if DEBUG is defined
#ifdef DEBUG
	Serial.begin(115200);
#endif //DEBUG
}

void loop() {
	//regular interval for SPI communication with ATX board
	if ((micros() - SpiMicrosBefore) > SpiMicroTime) {
		SpiMicrosBefore = micros();
		//reads Encoder position and ATX parameters
		Encoder.readPosition();
		Atx.sendAndReceiveAll();
		//runs the PI controllers
		calculateServoPosition();
		calculateBldcPosition();
	}
	//moves the stepper if needed. This function has to be called as often as possible for maximum responsiveness
	Stepper.run();

	//debugging messages if defined above
#ifdef DEBUG
	if ((millis() - debugMillisBefore) > debugMilliTime) {
		debugMillisBefore = millis();
		Serial.printf("Pos %d  int %d  F %d  Target %d FTarget %d\n", Atx.BldcMagneticPosition, BldcIntegral, BldcForce, Stepper.targetPosition(), BldcForceTarget);
		//Serial.printf("Position %ld  Force %d  Integral %d  TargetF %d\n", longServoPosition[0], Atx.servoForce[0], servoIntegral[0], servoForceTarget[0]);
		//Serial.printf("%d\t%d\n", Encoder.position, Atx.servoPosition[0]);
		//Serial.printf("Bldc: %d\tForce: %d\tTargetForce: %d\tIntegral: %d\tTargetPosition: %d\n", Atx.BldcMagneticPosition, BldcForce, BldcForceTarget, BldcIntegral, Stepper.distanceToGo());
		//Serial.printf("Force: %d\tTarget: %d\n", BldcForce, BldcPositionTarget);
	}
#endif // DEBUG
}

void calculateServoPosition() {

	for (int i = 0; i <= 1; i++) {
		//calculate target force
		if (Atx.servoPosition[i] < servoFreerunPosition[i]) {
			servoForceTarget[i] = servoForceFreerun[i];
		}
		else {
			servoForceTarget[i] = servoForceFreerun[i] + servoStiffness[i] * (Atx.servoPosition[i] - servoFreerunPosition[i]);
		}

		//calculate force delta and integral
		servoForceDelta[i] = servoForceTarget[i] - Atx.servoForce[i];
		if (longServoPosition[i] <= 0) {
			servoIntegral[i] = 0;
		}
		else {
			servoIntegral[i] += servoForceDelta[i];
		}

		//calculate new target position with PI controller
		longServoPosition[i] -= ((servoForceDelta[i] * servoPFactor[i])+(servoIntegral[i]*servoIFactor[i]));

		//check and correct for over/underflow of 16bit value with 32bit value mask
		if (longServoPosition[i] >= 65535) {
			longServoPosition[i] = 65535;
		}
		else if (longServoPosition[i] <= 0) {
			longServoPosition[i] = 0;
		}
		//sets new servo values and writes them to the ATX board
		Atx.servoPosition[i] = longServoPosition[i];
	}
}

void calculateBldcPosition() {
	//calculate corrected Bldc force
	//BldcForceRaw = Encoder.position - Atx.BldcMagneticPosition;
	//if (BldcForce <= -73) {
	//	BldcForceOffset++;
	//}
	//if (BldcForce >= 73) {
	//	BldcForceOffset--;
	//}
	//BldcForce = 146 * BldcForceOffset + BldcForceRaw;
	Encoder.position = map(Encoder.position, 0, 1023, 0, 2519);
	BldcForce = Encoder.position - Atx.BldcMagneticPosition;

	//calculate force target and delta
	if (Stepper.currentPosition() <= BldcFreerunPosition) {
		BldcForceTarget = BldcForceFreerun;
	}
	else {
		BldcForceTarget = BldcForceFreerun + (Stepper.currentPosition() - BldcFreerunPosition)*BldcStiffness;
	}
	BldcForceDelta = BldcForceTarget - BldcForce;
	
	if (Stepper.currentPosition() <= 0) {
		BldcIntegral = 0;
	}
	else {
		BldcIntegral += BldcForceDelta;
	}
	//calculate Steps to take
	Stepper.moveTo(Stepper.currentPosition() - BldcPFactor*BldcForceDelta-BldcIFactor*BldcIntegral);
	if (Stepper.targetPosition() <= 0) {
		Stepper.moveTo(0);
	}
}