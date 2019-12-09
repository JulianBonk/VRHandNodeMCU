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
#define servo1PFactor			5
#define servo1IFactor			0.005
#define servo1ForceFreerun		50
#define servo1Stiffness			0.005
#define servo1FreerunPosition	10000

#define servo2PFactor			5
#define servo2IFactor			0.005
#define servo2ForceFreerun		50
#define servo2Stiffness			0.005
#define servo2FreerunPosition	10000

//BLDC parameters
#define confBldcPFactor			5
#define confBldcIFactor			1
#define confBldcForceFreerun	15
#define confBldcStiffness		0.01
#define confBldcFreerunPosition	600
#define bldcAcceleration		100000
#define bldcSpeed				10000
#define DIR						D2
#define STEP					D1
#define bldcDefaultCurrent		0xFF

//SPI parameters
#define SPIFrequencyEncoder 1000000	//max frequency for encoder 1Mhz
#define SPIFrequencyAtx 1000000

//SPI slave select pins
const int CSAtxPin = D8;
const int CSEncoderPin = D4;