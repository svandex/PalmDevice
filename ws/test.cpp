#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/reader.h>
#include <rapidjson/writer.h>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include <mysqlx/xdevapi.h>

typedef websocketpp::server<websocketpp::config::asio> server;
using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

void on_message(server *s, websocketpp::connection_hdl hdl, server::message_ptr msg)
{
    //std::cout << msg->get_payload() << std::endl;

    rapidjson::Document d;
    if (d.Parse(msg->get_payload().c_str()).HasParseError())
    {
        s->send(hdl, "parse error", websocketpp::frame::opcode::TEXT);
        return;
    }
    std::cout << d["gender"].GetBool()<< std::endl;

    //MYSQL
    mysqlx::Session mysql_ss("localhost", 33060, "svandex", "y1ban@Hust");
    /*
    std::list<mysqlx::Schema> schemaList = mysql_ss.getSchemas();

    for (mysqlx::Schema sch : schemaList)
    {
        std::cout << sch.getName() << std::endl;
    }
    */

    std::string wt = "1";
    if (d["gender"].GetBool())
    {
        wt = "0";
    }
    std::string sql_stm = "insert table staffmgr(name,password,email,gender,speciality) values(" + std::string(d["name"].GetString()) + std::string(d["password"].GetString()) + d["email"].GetString() + wt + d["speciality"][0].GetString() + ");";

    std::cout << sql_stm << std::endl;

    auto rsets = mysql_ss.sql(sql_stm).execute();
    std::wcout << rsets.getWarning(0).getMessage().c_str();

    try
    {
        //test JSON msg
        rapidjson::StringBuffer sb;
        rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

        writer.StartObject();   // Between StartObject()/EndObject(),
        writer.Key("hello");    // output a key,
        writer.String("world"); // follow by a value.
        writer.Key("t");
        writer.Bool(true);
        writer.Key("f");
        writer.Bool(false);
        writer.Key("n");
        writer.Null();
        writer.Key("i");
        writer.Uint(123);
        writer.Key("pi");
        writer.Double(3.1416);
        writer.Key("a");
        writer.StartArray(); // Between StartArray()/EndArray(),
        for (unsigned i = 0; i < 4; i++)
            writer.Uint(i); // all values are elements of the array.
        writer.EndArray();
        writer.EndObject();

        s->send(hdl, std::string(sb.GetString()), websocketpp::frame::opcode::TEXT);
    }
    catch (websocketpp::exception const &e)
    {
        std::cout << e.what() << std::endl;
    }
}

int main(int argc, char *argv[])
{
    try{
    //server setting
    server tvrig_server;
    //tvrig_server.set_open_handshake_timeout(1000000);
    //tvrig_server.set_max_http_body_size(64000000);

    tvrig_server.set_error_channels(websocketpp::log::elevel::fatal);
    //tvrig_server.set_access_channels(websocketpp::log::alevel::app); // ^ websocketpp::log::alevel::frame_payload);

    //tvrig_server.set_http_handler(bind(&on_http, &tvrig_server, ::_1));
    tvrig_server.set_message_handler(bind(&on_message, &tvrig_server, ::_1, ::_2));

    tvrig_server.init_asio();
    tvrig_server.listen(websocketpp::lib::asio::ip::tcp::v4(), 9002);
    tvrig_server.start_accept();

    tvrig_server.run();
    }catch(websocketpp::exception &e){
        std::wcout<<e.what()<<std::endl;
    }
}
