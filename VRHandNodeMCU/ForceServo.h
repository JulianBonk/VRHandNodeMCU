// ForceServo.h

#ifndef _FORCESERVO_h
#define _FORCESERVO_h
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#endif

class ForceServo
{
public:
	ForceServo();
	~ForceServo();

	//functions
	void setPosition(long position);
	void getForce();
	void setCurrent(int current);
	//variables
	long position;
	long force;
	int current;

private:
	//functions

	//variables
};
