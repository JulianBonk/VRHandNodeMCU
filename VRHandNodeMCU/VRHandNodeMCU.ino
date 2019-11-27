/*
 Name:		VRHandNodeMCU.ino
 Created:	23.11.2019 19:00
 Author:	julia
*/

#include "configuration.h" //configure parameters
#include "ATXCommunication.h" //custom communication library
#include "EMS22A.h"	//custom encoder library

#include "A4988.h"
#include "SPI.h"

#define DEBUG

A4988 stepper(MOTOR_STEPS, DIR, STEP);
ATXCommunication Atx(CSAtxPin);
EMS22A Encoder(CSEncoderPin);


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
int BldcPositionTarget = 2000;
int BldcPositionDelta = 0;
int BldcForce = 0;
int BldcForceTarget = 0;
int BldcForceDelta = 0;
int BldcIntegral = 0;
int BldcPFactor = confBldcPFactor;
int BldcIFactor = confBldcIFactor;
int BldcForceFreerun = confBldcForceFreerun;
int BldcStiffness = confBldcStiffness;
int BldcFreerunPosition = confBldcFreerunPosition;


void setup() {
	//stepper setup
	stepper.begin(RPM);
	stepper.enable();
	stepper.setSpeedProfile(stepper.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

	//SPI setup
	SPI.begin();
	SPI.setFrequency(SPIFrequency);

	Atx.current = confBldcCurrent;

	//Serial setup only if DEBUG is defined
#ifdef DEBUG
	Serial.begin(115200);
	Serial.println("begin:");
#endif //DEBUG
}

//MAIN CODE

void loop() {
	//Encoder.readPosition();
	Atx.readAll();

#ifdef DEBUG
	Serial.printf("Data received\n Servo1 Force: %d\tServo2 Force: %d\tBLDC: %d\tCurrent: %d\n\n", Atx.servoForce[0], Atx.servoForce[1], Atx.BldcMagneticPosition, Atx.current);
#endif // DEBUG

	for (int i = 0; i < 2; i++) {
		servoForceDelta[i] = servoForceTarget[i] - Atx.servoForce[i];
		servoIntegral[i] += servoForceDelta[i];
		Atx.servoPosition[i] = servoPFactor[i] * servoForceDelta[i] + servoIFactor[i] * servoIntegral[i];
		if (Atx.servoPosition[i] <= servoFreerunPosition[i]) {
			servoForceTarget[i] = servoForceFreerun[i];
		}
		else {
			servoForceTarget[i] = Atx.servoPosition[i] * servoStiffness[i] + servoForceFreerun[i];
		}
	}

	BldcForce = BldcPositionTarget - Atx.BldcMagneticPosition;
	BldcForceDelta = BldcForceTarget - BldcForce;
	BldcIntegral += BldcForceDelta;
	BldcPositionTarget = BldcPFactor * BldcForceDelta + BldcIntegral * BldcForceDelta;
	if (BldcPositionTarget <= BldcFreerunPosition) {
		BldcForceTarget = BldcForceFreerun;
	}
	else {
		BldcForceTarget = BldcPositionTarget * BldcStiffness + BldcForceFreerun;
	}

	if (!stepper.nextAction()) {
		//stepper.startRotate(BldcPositionTarget);
	}

	//Atx.sendAndReceiveAll();
	//if (stepper.nextAction()) {
	//	delay(10);
	//}
	//else {
	//	//delay(1000);
	//	//move by x degree
	//	//stepper.startRotate(degree);
	//	stepper.move(BldcPositionTarget);
	//	BldcPositionTarget = BldcPositionTarget * -1;
	//}
		

#ifdef DEBUG
	//Serial.printf("PI Calculation completed\n\nServo1 Position: %d\tServo2 Position: %d, BLDC Position: %d, BLDC Current: %d\n\n", Atx.servoPosition[0], Atx.servoPosition[1], BldcPositionTarget, Atx.current);
#endif // DEBUG
}