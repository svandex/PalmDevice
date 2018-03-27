#include "tvUSB.h"
#include <stdexcept>

tvUSB::tvUSB()
{
	if (libusb_init(&ctx) != 0){
		throw std::logic_error("Cannot Initialise usb context");
	}

	libusb_get
}

tvUSB::~tvUSB()
{
	libusb_exit(ctx);
}
