#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> wsserver;

void on_message(websocketpp::connection_hdl hdl, wsserver::message_ptr msg);
