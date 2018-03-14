#include "PalmDeviceServer.h"
#include <regex>
#include <cpprest/streams.h>
#include <cpprest/filestream.h>
#include <unistd.h>

PalmDeviceGPIO PalmDeviceServer::gpio;

PalmDeviceServer::PalmDeviceServer()
{
}

PalmDeviceServer::PalmDeviceServer(utility::string_t url) :m_listener(url)
{
	m_listener.support(methods::GET, std::bind(&PalmDeviceServer::handle_get, this, std::placeholders::_1));
	m_listener.support(methods::PUT, std::bind(&PalmDeviceServer::handle_put, this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&PalmDeviceServer::handle_post, this, std::placeholders::_1));
	m_listener.support(methods::DEL, std::bind(&PalmDeviceServer::handle_delete, this, std::placeholders::_1));
}


PalmDeviceServer::~PalmDeviceServer()
{
}

void PalmDeviceServer::handle_get(http_request message)
{
	//ucout << message.to_string() << std::endl;
	ucout << message.absolute_uri().to_string() << std::endl;

	utility::string_t currentpath = get_current_dir_name();
//	ucout << currentpath << std::endl;
	http_response message_response;
	//Client needs html and javascript code to interact with server
	//Return two or more files back to client, each with a HTTP frame

	//URI "/" return the default index.html
	if (message.absolute_uri().to_string() == "/") {
		message_response.headers().set_content_type("text/html");
		utility::string_t index_html_path = currentpath + "/../res/index.html";
		//ucout << currentpath << std::endl;

		pplx::task<void> requestTask = Concurrency::streams::file_stream<uint8_t>::open_istream(index_html_path)

			.then([&message_response](Concurrency::streams::basic_istream<uint8_t> response_basic_istream) {
			message_response.set_body(response_basic_istream);
			return message_response;
		})
			.then([&message](http_response message_response) {
			message_response.set_status_code(status_codes::OK);
			message.reply(message_response);

			const int num = 5;
			gpio.ledFlash(num, 1);
		});

		try {
			requestTask.wait();
		}
		catch (std::exception const &e) {
			std::wcout << "Exception: " << e.what() << std::endl;
			//Error occured, internal error return.
			message_response.set_status_code(status_codes::InternalError);
			message.reply(message_response);
			return;
		}
	}

	//URI "/canvasjs.min.js" return the canvasjs.min.js file
	if (message.absolute_uri().to_string() == "/canvasjs.min.js") {
		message_response.headers().set_content_type("text/javascript");
		utility::string_t js_path = currentpath + "/../res/canvasjs.min.js";

		pplx::task<void> requestTask = Concurrency::streams::file_stream<uint8_t>::open_istream(js_path)

			.then([&message_response](Concurrency::streams::basic_istream<uint8_t> response_basic_istream) {
			message_response.set_body(response_basic_istream);
			return message_response;
		})
			.then([&message](http_response message_response) {
			message_response.set_status_code(status_codes::OK);
			message.reply(message_response);
		});

		try {
			requestTask.wait();
		}
		catch (std::exception const &e) {
			std::wcout << "Exception: " << e.what() << std::endl;
			//Error occured, internal error return.
			message_response.set_status_code(status_codes::InternalError);
			message.reply(message_response);
			return;
		}
	}

	//URI "/favicon.ico" return the ico file
	if (message.absolute_uri().to_string() == "/favicon.ico") {
		message_response.headers().set_content_type("image/x-icon");
		utility::string_t js_path = currentpath + "/../res/favicon.ico";

		pplx::task<void> requestTask = Concurrency::streams::file_buffer<uint8_t>::open(js_path, std::ios_base::in)
			.then([&message_response](Concurrency::streams::streambuf<uint8_t> response_streambuf) {
			auto temp_stream = Concurrency::streams::basic_istream<uint8_t>(response_streambuf);
			message_response.set_body(temp_stream);
			return message_response;
		})
			.then([&message](http_response message_response) {
			message_response.set_status_code(status_codes::OK);
			message.reply(message_response);
		});

		try {
			requestTask.wait();
		}
		catch (std::exception const &e) {
			std::wcout << "Exception: " << e.what() << std::endl;
			//Error occured, internal error return.
			message_response.set_status_code(status_codes::InternalError);
			message.reply(message_response);
			return;
		}
	}
	//Print response and replay to the request
	//ucout << message_response.to_string() << std::endl;
	//ucout <<"Has responed to "<< message.absolute_uri().to_string() << std::endl;
}

void PalmDeviceServer::handle_put(http_request message)//If request has content body
{
	ucout << message.to_string() << std::endl;
	message.reply(status_codes::OK);
}

void PalmDeviceServer::handle_post(http_request message)
{
	const int num = 6;
	//ucout << message.to_string() << std::endl;
	std::string temp = message.extract_string(true).get();

	//use regex to get delaytime
	std::regex tempRegex(R"([0-9]{1,2})");
	std::cmatch tempSm;

	message.reply(status_codes::OK);
	if (std::regex_search(temp.c_str(), tempSm, tempRegex)) {
	/*	ucout << tempSm.size() << std::endl;
		ucout << tempSm[0].length() << std::endl;
		ucout << tempSm[0].str() << std::endl;
		*/
	}
	if (tempSm.size() > 0) {
		gpio.ledFlash(num, std::atoi(tempSm[0].str().c_str()));
	}
}

void PalmDeviceServer::handle_delete(http_request message)
{
	ucout << message.to_string() << std::endl;
	message.reply(status_codes::OK);
}
