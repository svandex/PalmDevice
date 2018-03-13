#include "PalmDeviceGPIO.h"
#include <iostream>

int PalmDeviceGPIO::numOfInstance = 0;

PalmDeviceGPIO::PalmDeviceGPIO()
{
	if (numOfInstance++ > 1) {
		std::cout << "Only One Instance should be exist.";
		return;
	}
	gpioInitialise();
}


PalmDeviceGPIO::~PalmDeviceGPIO()
{
	gpioTerminate();
}

void PalmDeviceGPIO::ledFlash(const int &ledPinNum, int delaytime)
{
	gpioSetMode(ledPinNum, PI_OUTPUT);

	for (int i = 0; i < 3; i++) {
		gpioWrite(ledPinNum, PI_HIGH);
		gpioDelay(delaytime*100000);
		gpioWrite(ledPinNum, PI_LOW);
		gpioDelay(delaytime*100000);
	}
}
