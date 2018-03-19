#pragma once

#include <cpprest/json.h>
#include <cpprest/http_listener.h>
#include <cpprest/uri.h>
#include <cpprest/asyncrt_utils.h>

using namespace web;
using namespace http;
using namespace utility;
using namespace web::http::experimental::listener;

#include "tvGPIO.hpp"

class tvHTTP
{
public:
	tvHTTP();
	tvHTTP(utility::string_t url);

	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }
	
	void on_initialize(const string_t& address);
	void on_shutdown();
	~tvHTTP();
private:
	void handle_get(http_request message);
	void handle_put(http_request message);
	void handle_post(http_request message);
	void handle_delete(http_request message);

	void send_file_to_client(const http_request &message,http_response &message_response,const char* content_type, const char* related_path);
	http_listener m_listener;
};