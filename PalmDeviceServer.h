#pragma once

#include <cpprest/json.h>
#include <cpprest/http_listener.h>
#include <cpprest/uri.h>
#include <cpprest/asyncrt_utils.h>

using namespace web;
using namespace http;
using namespace utility;
using namespace web::http::experimental::listener;

#include "PalmDeviceGPIO.h"

class PalmDeviceServer
{
public:
	PalmDeviceServer();
	PalmDeviceServer(utility::string_t url);

	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }

	~PalmDeviceServer();
private:
	void handle_get(http_request message);
	void handle_put(http_request message);
	void handle_post(http_request message);
	void handle_delete(http_request message);

	http_listener m_listener;
	static PalmDeviceGPIO gpio;
};

std::unique_ptr<PalmDeviceServer> g_http;

void on_initialize(const string_t& address);
void on_shutdown();