#include "tvGPIO.hpp"
#include <iostream>
#include <fstream>
#include <bitset>

#include <sys/stat.h>
#include <unistd.h>


std::string gpioPrompt = "[GPIO]: ";
#define gOut std::cout<<gpioPrompt

int tvGPIO::numOfInstance = 0;

tvGPIO::tvGPIO()
{
	if (numOfInstance++ > 1) {
		gOut << "Only One Instance should be exist.";
		return;
	}
	gpioInitialise();
	gpioSetMode(TC_DC, PI_OUTPUT);
	gpioWrite(TC_DC, PI_LOW);
}


tvGPIO::~tvGPIO()
{
	gpioTerminate();
}

void tvGPIO::ledFlash(const int &ledPinNum, int delaytime)
{
	gpioSetMode(ledPinNum, PI_OUTPUT);

	for (int i = 0; i < 1; i++) {
		gpioWrite(ledPinNum, PI_HIGH);
		gpioDelay(delaytime * 100000);
		gpioWrite(ledPinNum, PI_LOW);
		gpioDelay(delaytime * 100000);
	}
}

std::unique_ptr<uint16_t> tvGPIO::daqByNum() const
{
	uint16_t element;
	std::unique_ptr<uint16_t> vElement(new uint16_t[TC_DNUM]);
	//change permission to let data aqusition card to write into it
	if (chmod("/dev/ttyUSB0", S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
		gOut << "Cannot Change Permission : /dev/ttyUSB0" << std::endl
			<< "Aquired Data cannot write into /dev/ttyUSB0" << std::endl;
		return nullptr;
	}

	std::fstream ftty("/dev/ttyUSB0", std::ios::in);
	if (ftty.is_open()) {
		this->daqStart();
		//std::cout << "[GPIO: ]";
		for (uint16_t num = 0; num < TC_DNUM; num++) {
			ftty >> element;
			//gOut << element << ", ";
			if (element > std::bitset<TC_PREC>(0xffff).to_ulong()) {//data maximum not exceed 2^TC_PREC
				num--;
				continue;
			}
			*(vElement.get() + num) = element;
		}
		this->daqStop();

		gOut << "Aquired data from Arduino in " << TC_DNUM * 2 << " bytes." << std::endl;
		gOut << "Remained " << ftty.rdbuf()->in_avail() << " bytes unread" << std::endl;

		ftty.sync();
		ftty.close();
		return vElement;
	}
	else {//not openend
		gOut << "USB port is not connected." << std::endl;
	}

	if (chmod("/dev/ttyUSB0", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) < 0) {
		gOut << "Cannot Change Permission : /dev/ttyUSB0" << std::endl
			<< "May have risk that others write into this file." << std::endl;
	}
	return nullptr;
}

void tvGPIO::daqByTime()
{
	uint16_t element;
	//change permission to let data aqusition card to write into it
	if (chmod("/dev/ttyUSB0", S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
		gOut << "Cannot Change Permission : /dev/ttyUSB0" << std::endl
			<< "Aquired Data cannot write into /dev/ttyUSB0" << std::endl;
		return;
	}

	//start aquiring data
	this->daqStart();
	gpioDelay(2000);
	//---------------IMPORTANT----------------
	// gpioDelay, gpio controller delayed but program will go on
	//---------------IMPORTANT---------------
	this->daqStop();
	// wait seconds
	usleep(3000);

	std::fstream ftty("/dev/ttyUSB0", std::ios::in);
	if (ftty.is_open()) {//opened
		//collect data from /dev/ttyUSB0
		uint16_t bufSz = ftty.rdbuf()->in_avail();
		gOut << "Has aquired " << bufSz << " in bytes." << std::endl;
		for (uint16_t num = 0; num < bufSz / 2; num++) {
			ftty >> element;
			std::cout << element << ", ";
		}
		gOut << std::endl << "DAQ end." << std::endl;
	}
	else {//not openend
		gOut << "USB port is not connected." << std::endl;
	}
	if (chmod("/dev/ttyUSB0", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) < 0) {
		gOut << "Cannot Change Permission : /dev/ttyUSB0" << std::endl
			<< "May have risk that others write into this file." << std::endl;
	}
}
