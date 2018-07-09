#include "tvWS.h"
#include "tvHTTP.h"

#include <memory>

std::string wsPrompt = "[WS]: ";
#define wsOut std::cout<<wsPrompt

extern tvGPIO gpio;
extern std::unique_ptr<tvHTTP> g_http;

void on_message(wsserver* s, websocketpp::connection_hdl hdl, wsserver::message_ptr msg)
{/*
	auto SendData = gpio.daqByNum();
	if (SendData == nullptr) {
		wsOut << "No data has been aquired." << std::endl;
		return;
	}
	//show what has been aquired.
	wsOut << " Data: ";
	for (uint16_t num = 0; num < TC_DNUM; num++) {
		std::cout << "  " << *(SendData.get() + num);
	}
	std::cout << std::endl;

	//send data to client
	auto con = s->get_con_from_hdl(hdl);
	con->send((void*)SendData.get(), sizeof(TC_DNUM * 2), websocketpp::frame::opcode::binary);
	//con->send("hello world", 11, websocketpp::frame::opcode::text);
        */
}

tvWS::tvWS()
{
	data_server.init_asio();
	data_server.set_reuse_addr(true);

	data_server.set_message_handler(websocketpp::lib::bind(&on_message, &data_server, ::_1, ::_2));
	data_server.set_access_channels(websocketpp::log::alevel::all);
	data_server.set_error_channels(websocketpp::log::elevel::all);

	data_server.listen(wsPort);//websocet server port number
}

void tvWS::on_initialize()
{
	try {
		wsOut << "Listening request at " << std::endl
			<< "		ws://10.10.10.1:60001 on ethernet" << std::endl
			<< "		ws://10.42.0.1:60001 on wlan" << std::endl;
		data_server.start_accept();
		data_server.run();
	}
	catch (websocketpp::exception const&e) {
		wsOut << "exception: " << e.what() << std::endl;
	}
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
