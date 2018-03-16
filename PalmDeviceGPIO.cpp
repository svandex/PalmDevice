#include "PalmDeviceGPIO.h"
#include <iostream>
#include <fstream>

int PalmDeviceGPIO::numOfInstance = 0;
const int SensorControlPin = 26;

PalmDeviceGPIO::PalmDeviceGPIO()
{
	if (numOfInstance++ > 1) {
		std::cout << "Only One Instance should be exist.";
		return;
	}
	gpioInitialise();
	gpioSetMode(SensorControlPin, PI_OUTPUT);
	gpioWrite(SensorControlPin, PI_LOW);
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
		gpioDelay(delaytime * 100000);
		gpioWrite(ledPinNum, PI_LOW);
		gpioDelay(delaytime * 100000);
	}
}

void PalmDeviceGPIO::sensorAction()
{
	uint16_t element;
	std::fstream ftty("/dev/ttyUSB0", std::ios::in);
	if (ftty.is_open()) {//opened
		gpioWrite(SensorControlPin, PI_HIGH);//start aquiring data
		for (uint8_t num = 0; num < 5; num++) {
			ftty >> element;
			std::cout << element << std::endl;
		}
		gpioWrite(SensorControlPin, PI_LOW);//end aquiring data
		//std::cout << "Has aquired " << ftty.rdbuf()->in_avail() << " in bytes." << std::endl;
	}
	else {//not openend
		std::cout << "USB port is not connected." << std::endl;
		return;
	}
}
