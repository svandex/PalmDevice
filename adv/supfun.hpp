#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <xlnt/xlnt.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

extern std::string rootpath;

namespace juncheng
{
void on_message(server *s, websocketpp::connection_hdl hdl, server::message_ptr msg);
void on_http(server *s, websocketpp::connection_hdl hdl);
template <typename encoding>
std::string send_file_to_client(server::connection_ptr &con, const std::string &resURI, const std::string &content_type);
} // namespace juncheng

