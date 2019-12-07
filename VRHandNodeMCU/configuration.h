// configuration.h

#ifndef _CONFIGURATION_h
#define _CONFIGURATION_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#endif

//servo parameters
#define servo1PFactor			1
#define servo1IFactor			0
#define servo1ForceFreerun		20
#define servo1Stiffness			1
#define servo1FreerunPosition	1

#define servo2PFactor			1
#define servo2IFactor			1
#define servo2ForceFreerun		1
#define servo2Stiffness			1
#define servo2FreerunPosition	1

//BLDC parameters
#define confBldcPFactor			-1
#define confBldcIFactor			0
#define confBldcForceFreerun	5
#define confBldcStiffness		1
#define confBldcFreerunPosition	1
#define bldcAcceleration		10000
#define bldcSpeed				10

//stepper parameters
#define MOTOR_STEPS 2520
#define RPM 200
#define MOTOR_ACCEL 20000
#define MOTOR_DECEL 20000
#define DIR D2
#define STEP D1

//SPI parameters
#define SPIFrequencyEncoder 1000000	//max frequency for encoder 1Mhz
#define SPIFrequencyAtx 4000000

//SPI slave select pins
const int CSAtxPin = D8;
const int CSEncoderPin = D4;