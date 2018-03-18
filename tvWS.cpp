#include "tvWS.hpp"
#include "tvHTTP.hpp"

#include <vector>

constexpr char* wsPrompt = "[WS]: ";
#define wsOut std::cout<<wsPrompt

extern tvGPIO gpio;
extern std::unique_ptr<tvHTTP> g_http;

void OnMessage(websocketpp::connection_hdl hdl, wsserver::message_ptr msg)
{
	wsOut << " get message." << std::endl;
	std::vector<uint16_t> SendData = gpio.daqByNum();
	wsOut << " Aquired data from Adruino " << std::endl << wsPrompt;
	std::for_each(SendData.begin(), SendData.end(), [](uint16_t element) {
		std::cout << element << ", ";
	});
	std::cout<<std::endl;
}

tvWS::tvWS()
{
		data_server.set_message_handler(&OnMessage);
		data_server.set_access_channels(websocketpp::log::alevel::all);
		data_server.set_error_channels(websocketpp::log::elevel::all);
		data_server.init_asio();
		data_server.listen(wsPort);//websocet server port number
		data_server.start_accept();
}

void tvWS::on_initialize()
{
	wsOut << "Listening request at "<<std::endl
		<<"		ws://10.10.10.1:60001 on ethernet"<<std::endl
		<<"		ws://10.42.0.1:60001 on wlan" << std::endl;
	data_server.run();
}

void tvWS::on_shutdown()
{
	data_server.stop();
}

tvWS::~tvWS()
{
	if (data_server.stopped()) {
		wsOut << "Websocket Server has been stopped." << std::endl;
	}
	else {
		wsOut << "Websocket Server is stopping..." << std::endl;
		data_server.stop();
	}
}
