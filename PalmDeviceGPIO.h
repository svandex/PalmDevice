#pragma once

#include <pigpio.h>

class PalmDeviceGPIO
{
public:
	PalmDeviceGPIO();
	PalmDeviceGPIO(PalmDeviceGPIO &) = delete; //cannot copy gpio instance, only one exists
	~PalmDeviceGPIO();
	void ledFlash(const int &ledPinNum, int delaytime);
private:
	static int numOfInstance;//no more than one instance
};

