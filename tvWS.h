#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <cpprest/asyncrt_utils.h>

typedef websocketpp::server<websocketpp::config::asio> wsserver;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

void on_message(wsserver *s, websocketpp::connection_hdl hdl, wsserver::message_ptr msg);

class tvWS {
public:
	tvWS();
	tvWS(tvWS const &) = delete;
	void on_initialize();
	void on_shutdown();

	~tvWS();
private:
	wsserver data_server;
	const uint16_t wsPort = 60001;
};
