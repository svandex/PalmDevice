#pragma once

#include "tvConfig.h"    

#include <pigpio.h>
#include <vector>
#include <memory>
#include <iostream>

//Serial Port 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <pthread.h>

#include <memory.h>

class tvGPIO
{
public:
	tvGPIO();
	tvGPIO(tvGPIO &) = delete; //cannot copy gpio instance, only one exists
	~tvGPIO();
	void ledFlash(const int &ledPinNum, int delaytime);
        std::vector<uint16_t> daqByNum() const;
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
        int m_fd;//file discriptor
        termios m_termOption;//Terminal IO option
};

