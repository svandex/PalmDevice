#pragma once

#include <libusb-1.0\libusb.h>

class tvUSB {
public:
	tvUSB();
	~tvUSB();
private:
	libusb_context * ctx;
};
