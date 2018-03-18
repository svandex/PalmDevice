#include "tvHTTP.hpp"
#include <regex>
#include <cpprest/streams.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include <unistd.h>
#include <thread>

constexpr char* httpPrompt = "[HTTP]: ";
#define hOut std::cout<<httpPrompt

extern tvGPIO gpio;
extern std::unique_ptr<tvHTTP> g_http;

tvHTTP::tvHTTP()
{
}

tvHTTP::tvHTTP(utility::string_t url) :m_listener(url)
{
	m_listener.support(methods::GET, std::bind(&tvHTTP::handle_get, this, std::placeholders::_1));
	m_listener.support(methods::PUT, std::bind(&tvHTTP::handle_put, this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&tvHTTP::handle_post, this, std::placeholders::_1));
	m_listener.support(methods::DEL, std::bind(&tvHTTP::handle_delete, this, std::placeholders::_1));
}


tvHTTP::~tvHTTP()
{
}

void tvHTTP::handle_get(http_request message)
{
	gpio.ledFlash(gpio.htmlStatePin, 1);
	//hOut << message.to_string() << std::endl;
	hOut << message.absolute_uri().to_string() << std::endl;

	utility::string_t currentpath = get_current_dir_name();
	//	hOut << currentpath << std::endl;
	http_response message_response;
	//Client needs html and javascript code to interact with server
	//Return two or more files back to client, each with a HTTP frame

	//URI "/" return the default index.html
	if (message.absolute_uri().to_string() == "/") {
		message_response.headers().set_content_type("text/html");
		utility::string_t index_html_path = currentpath + "/res/index.html";
		//hOut << currentpath << std::endl;

		pplx::task<void> requestTask = Concurrency::streams::file_stream<uint8_t>::open_istream(index_html_path)

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

	//URI "/canvasjs.min.js" return the canvasjs.min.js file
	if (message.absolute_uri().to_string() == "/canvasjs.min.js") {
		message_response.headers().set_content_type("text/javascript");
		utility::string_t js_path = currentpath + "/res/canvasjs.min.js";

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
		utility::string_t js_path = currentpath + "/res/favicon.ico";

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
	//hOut << message_response.to_string() << std::endl;
	//hOut <<"Has responed to "<< message.absolute_uri().to_string() << std::endl;
}

void tvHTTP::handle_put(http_request message)//If request has content body
{
	hOut << message.to_string() << std::endl;
	message.reply(status_codes::OK);
}

void tvHTTP::handle_post(http_request message)
{/*
	//hOut << message.to_string() << std::endl;
	std::string temp = message.extract_string(true).get();

	//use regex to get delaytime
	std::regex tempRegex(R"([0-9]{1,2})");
	std::cmatch tempSm;

	message.reply(status_codes::OK);
	if (std::regex_search(temp.c_str(), tempSm, tempRegex)) {
	}
	if (tempSm.size() > 0) {
		gpio.daqByNum();
	}*/
	auto temp = message.extract_json()
		.then([](web::json::value resultValue) {
		auto result = resultValue.at(U("Data Aquired Number"));
		std::cout << "Parsed JSON data: " << result << std::endl;
	});
	try {
		temp.wait();
	}
	catch (std::exception const &e) {
		std::wcout << "Exception: " << e.what() << std::endl;
		std::wcout << "This is not json file format." << std::endl;
		//Error occured, internal error return.
		return;
	}
}

void tvHTTP::handle_delete(http_request message)
{
	hOut << message.to_string() << std::endl;
	message.reply(status_codes::OK);
}

void tvHTTP::on_initialize(const string_t& address) {
	uri_builder uri(address);
	// address:port/Action
	//uri.append_path(U(""));
	auto addr = uri.to_uri().to_string();
	g_http = std::unique_ptr<tvHTTP>(new tvHTTP(addr));
	g_http->open().wait();

	hOut << utility::string_t(U("Listening for request at: ")) <<std::endl
		<<"		http://"<<addr <<" on both ethernet and wlan"<< std::endl;
}

void tvHTTP::on_shutdown() {
	g_http->close().wait();
	return;
}
