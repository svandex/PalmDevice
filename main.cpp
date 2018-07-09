/*
Author: Qiu Juncheng
Company Email: qiujuncheng@saicmotor.com
Personal Email: svandex@outlook.com
GitHub: https://github.com/svandex
Date: 2018-02-08

Package Dependency:
	cpprest						http://github.com/microsoft/cpprestsdk
	boost						http://boost.org
	pigpio						http://github.com/joan2937/pigpio
	libmysqlcppconn-dev			http://dev.mysql.com/doc/connector-cpp/en
	websocketpp				https://github.com/zaphoyd/websocketpp
	libwebsockets				https://github.com/warmcat/libwebsockets

	recommend to use Visual Studio For Linux to remote debug Raspberry Pi 3, http://svandex.github.io
	recommend to copy /usr/include/ and /usr/local/include to your pc for intellisense
	recommend to use APT to install dependent packages

	* make sure you have access to root user, so you can remote deploy and run your program on Raspberry Pi 3

Feature:
	http server
	gamepad controller
	servo control
*/
#include "tvHTTP.h"
#include "tvWS.h"

#include <thread>

tvGPIO gpio;					//only one gpio instance could exist, take care about multi-thread situation
std::unique_ptr<tvWS> g_WS;			//websocket server instance
std::unique_ptr<tvHTTP> g_http;			//http server instance

int main(int argc, char* argv[]) {

	std::thread wsThread([]() {
		g_WS = std::unique_ptr<tvWS>(new tvWS());
		g_WS->on_initialize();
	});
	wsThread.detach();

	//start cpprest http server
	utility::string_t port = U("60000");
	utility::string_t address = U("http://0.0.0.0:");
	address.append(port);

	g_http->on_initialize(address);
	std::cout << "Press Enter to exit..." << std::endl;

	uint8_t line;
	std::cin >> line;

	g_http->on_shutdown();
	g_WS->on_shutdown();
	return EXIT_SUCCESS;
}
