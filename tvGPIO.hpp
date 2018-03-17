#pragma once

#include <pigpio.h>

class tvGPIO
{
public:
	tvGPIO();
	tvGPIO(tvGPIO &) = delete; //cannot copy gpio instance, only one exists
	~tvGPIO();
	void ledFlash(const int &ledPinNum, int delaytime);
	void sensorAction();
private:
	static int numOfInstance;//no more than one instance
};

