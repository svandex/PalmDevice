#pragma once

#include <libusb-1.0/libusb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <pthread.h>

class tvUSB {
public:
	tvUSB();
	~tvUSB();
private:
	libusb_context * ctx;
};
