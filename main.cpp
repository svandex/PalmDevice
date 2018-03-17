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
	websocketcpp				https://github.com/zaphoyd/websocketpp

	recommend to use Visual Studio For Linux to remote debug Raspberry Pi 3, http://svandex.github.io
	recommend to copy /usr/include/ and /usr/local/include to your pc for intellisense
	recommend to use APT to install dependent packages

	* make sure you have access to root user, so you can remote deploy and run your program on Raspberry Pi 3

Feature:
	http server
	gamepad controller
	servo control
*/
#include "tvHTTP.hpp"
#include "tvWS.hpp"

#include <thread>

std::unique_ptr<tvHTTP> g_http;

int main(int argc, char* argv[]) {
	//start websocket server
	wsserver data_server;
	data_server.set_message_handler(&on_message);

	data_server.init_asio();
	data_server.listen(60001);//websocet server port number
	data_server.start_accept();

	std::thread wsThread([&data_server]() {
		data_server.run();
	});
	wsThread.detach();

	//start cpprest http server
	utility::string_t port = U("60000");

	//utility::string_t address = U("http://localhost:");
	utility::string_t address = U("http://0.0.0.0:");
	address.append(port);

	on_initialize(address);
	std::cout << "Press Enter to exit..." << std::endl;

	std::string line;
	std::getline(std::cin, line);

	on_shutdown();
	//data_server in wsThread, will there be memory leaked?
	if (data_server.is_listening()) {
		data_server.stop();
	}
	return EXIT_SUCCESS;
}
