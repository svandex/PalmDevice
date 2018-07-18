#include <iostream>
#include <xlnt/xlnt.hpp>

std::string rootpath;

#include <supfun.hpp>


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Parameter Error, Two Arguments Required" << std::endl;
    }
    rootpath = argv[1];
    try
    {
        //server setting
        server tvrig_server;
        tvrig_server.set_open_handshake_timeout(1000000);
        tvrig_server.set_max_http_body_size(64000000);

        tvrig_server.set_error_channels(websocketpp::log::elevel::fatal);
        tvrig_server.set_access_channels(websocketpp::log::alevel::app ^ websocketpp::log::alevel::frame_payload);

        tvrig_server.set_http_handler(bind(&juncheng::on_http, &tvrig_server, ::_1));
        tvrig_server.set_message_handler(bind(&juncheng::on_message, &tvrig_server, ::_1, ::_2));

        tvrig_server.init_asio();
        tvrig_server.listen(websocketpp::lib::asio::ip::tcp::v4(), 9002);
        tvrig_server.start_accept();

        tvrig_server.run();
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}