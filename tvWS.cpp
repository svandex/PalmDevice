#include "tvWS.hpp"
#include "tvHTTP.hpp"

extern std::unique_ptr<tvHTTP> g_http;

void OnMessage(websocketpp::connection_hdl hdl, wsserver::message_ptr msg)
{
	std::cout << "[WS]: get message." << std::endl;
}

tvWS::tvWS()
{
		data_server.set_message_handler(&OnMessage);
		data_server.set_access_channels(websocketpp::log::alevel::all);
		data_server.set_error_channels(websocketpp::log::alevel::all);
		data_server.init_asio();
		data_server.listen(60001);//websocet server port number
		data_server.start_accept();
}

void tvWS::on_initialize()
{
	std::cout << "Websocket Server is runiing at ws://10.10.10.1:60001" << std::endl;
	data_server.run();
}

tvWS::~tvWS()
{
	std::cout << "Websocket Server is stopping..." << std::endl;
	data_server.stop();
}
