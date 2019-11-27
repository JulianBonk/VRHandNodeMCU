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
#define servo1IFactor			1
#define servo1ForceFreerun		1
#define servo1Stiffness			1
#define servo1FreerunPosition	1

#define servo2PFactor			1
#define servo2IFactor			1
#define servo2ForceFreerun		1
#define servo2Stiffness			1
#define servo2FreerunPosition	1

//BLDC parameters
#define confBldcPFactor			1
#define confBldcIFactor			1
#define confBldcForceFreerun	1
#define confBldcStiffness		1
#define confBldcFreerunPosition	1
#define confBldcCurrent			255

//stepper parameters
#define MOTOR_STEPS 2520
#define RPM 120
#define MOTOR_ACCEL 2000
#define MOTOR_DECEL 1000
#define DIR D2
#define STEP D1

//SPI parameters
#define SPIFrequency 1000000	//max frequency for encoder 1Mhz

//SPI slave select pins
const int CSAtxPin = D8;
const int CSEncoderPin = D3;