#pragma once

#include <pigpio.h>
#include <vector>

class tvGPIO
{
public:
	tvGPIO();
	tvGPIO(tvGPIO &) = delete; //cannot copy gpio instance, only one exists
	~tvGPIO();
	void ledFlash(const int &ledPinNum, int delaytime);
	std::vector<uint16_t> daqByNum();
	void daqByTime();

	const int daqStatePin = 6;//if daq is undergoing
	const int htmlStatePin = 5;//if http server is transmitting files
	int daqByNum_Number = 30;//how many number of data is aquired
	const int daqByTime_Time = 3000;//ms
private:
	static int numOfInstance;//no more than one instance
};

