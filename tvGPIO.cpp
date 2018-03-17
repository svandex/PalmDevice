#include "tvGPIO.hpp"
#include <iostream>
#include <fstream>

#include <sys/stat.h>

int tvGPIO::numOfInstance = 0;
const int SensorControlPin = 26;

tvGPIO::tvGPIO()
{
	if (numOfInstance++ > 1) {
		std::cout << "Only One Instance should be exist.";
		return;
	}
	gpioInitialise();
	gpioSetMode(SensorControlPin, PI_OUTPUT);
	gpioWrite(SensorControlPin, PI_LOW);
}


tvGPIO::~tvGPIO()
{
	gpioTerminate();
}

void tvGPIO::ledFlash(const int &ledPinNum, int delaytime)
{
	gpioSetMode(ledPinNum, PI_OUTPUT);

	for (int i = 0; i < 3; i++) {
		gpioWrite(ledPinNum, PI_HIGH);
		gpioDelay(delaytime * 100000);
		gpioWrite(ledPinNum, PI_LOW);
		gpioDelay(delaytime * 100000);
	}
}

void tvGPIO::sensorAction()
{
	uint16_t element;
	//change permission to let data aqusition card to write into it
	if (chmod("/dev/ttyUSB0", S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
		std::cout << "Cannot Change Permission : /dev/ttyUSB0" << std::endl
			<< "Aquired Data cannot write into /dev/ttyUSB0" << std::endl;
		return;
	}

	//start aquiring data
	gpioWrite(SensorControlPin, PI_HIGH);
	gpioDelay(500);
	//---------------IMPORTANT----------------
	// gpioDelay, gpio controller delayed but program will go on
	//---------------IMPORTANT---------------
	gpioWrite(SensorControlPin, PI_LOW);//end aquiring data
	// wait seconds
	time_sleep(3);

	std::fstream ftty("/dev/ttyUSB0", std::ios::in);
	if (ftty.is_open()) {//opened
		//collect data from /dev/ttyUSB0
		auto bufSz = ftty.rdbuf()->in_avail();
		std::cout << "Has aquired " << bufSz << " in bytes." << std::endl;
		for (uint8_t num = 0; num < bufSz / 2; num++) {
			ftty >> element;
			std::cout << element << std::endl;
		}

		if (chmod("/dev/ttyUSB0", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) < 0) {
			std::cout << "Cannot Change Permission : /dev/ttyUSB0" << std::endl
				<< "May have risk that others write into this file." << std::endl;
		}
	}
	else {//not openend
		std::cout << "USB port is not connected." << std::endl;
		if (chmod("/dev/ttyUSB0", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) < 0) {
			std::cout << "Cannot Change Permission : /dev/ttyUSB0" << std::endl
				<< "May have risk that others write into this file." << std::endl;
		}
		return;
	}
}
