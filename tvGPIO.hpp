#pragma once

#include <pigpio.h>
#include <vector>
#include <memory>

#include "tvConfig.hpp"

class tvGPIO
{
public:
	tvGPIO();
	tvGPIO(tvGPIO &) = delete; //cannot copy gpio instance, only one exists
	~tvGPIO();
	void ledFlash(const int &ledPinNum, int delaytime);
	std::unique_ptr<uint16_t> daqByNum() const;
	void daqByTime();
protected:
	void daqStart() const{
		gpioWrite(TC_DC, PI_HIGH);
		gpioWrite(TC_DS, PI_HIGH);
	}
	void daqStop() const{
		gpioWrite(TC_DC, PI_LOW);
		gpioWrite(TC_DS, PI_LOW);
	}
private:
	static int numOfInstance;//no more than one instance
};

