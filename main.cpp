/*
Author1: Qiu Juncheng
Company Email: qiujuncheng@saicmotor.com
Personal Email: svandex@outlook.com
GitHub: https://github.com/svandex
Date: 2018-02-08

Author2: Jiang Lifu
Company Email: jianglifu@saicmotor.com
Personal Email: 2638696515@qq.com
GitHub: https://github.com/FrogGuaGuaGua
Date: 2018-03-08

Package Dependency:
	cpprest						http://github.com/microsoft/cpprestsdk
	boost						http://boost.org
	pigpio						http://github.com/joan2937/pigpio
	libmysqlcppconn-dev			http://dev.mysql.com/doc/connector-cpp/en

	recommend to use Visual Studio For Linux to remote debug Raspberry Pi 3, http://svandex.github.io
	recommend to copy /usr/include/ and /usr/local/include to your pc for intellisense
	recommend to use APT to install dependent packages

	* make sure you have access to root user, so you can remote deploy and run your program on Raspberry Pi 3

Feature:
	http server
	gamepad controller
	servo control
*/
#include "PalmDeviceServer.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace web::http::experimental::listener;

std::unique_ptr<PalmDeviceServer> g_http;

void on_initialize(const string_t& address) {
	uri_builder uri(address);
	// address:port/Action
	//uri.append_path(U(""));
	auto addr = uri.to_uri().to_string();
	g_http = std::unique_ptr<PalmDeviceServer>(new PalmDeviceServer(addr));
	g_http->open().wait();

	ucout << utility::string_t(U("Listening for request at: ")) << addr << std::endl;
}

void on_shutdown() {
	g_http->close().wait();
	return;
}

int main(int argc, char* argv[]) {
	utility::string_t port = U("60000");
	if (argc == 2) {
		port = argv[1];
	}

	//utility::string_t address = U("http://localhost:");
	utility::string_t address = U("http://0.0.0.0:");
	address.append(port);

	on_initialize(address);
	std::cout << "Press Enter to exit..." << std::endl;

	std::string line;
	std::getline(std::cin, line);

	on_shutdown();
	return EXIT_SUCCESS;
}
