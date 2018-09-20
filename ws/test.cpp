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

void on_message(server *s, websocketpp::connection_hdl hdl, server::message_ptr msg) try
{
    //    std::cout << msg->get_payload() << std::endl;
    try
    {
        rapidjson::Document d;
        if (d.Parse(msg->get_payload().c_str()).HasParseError())
        {
            s->send(hdl, "{\"rapidjson\":\"parse error\"}", websocketpp::frame::opcode::TEXT);
            return;
        }

        //MYSQL
        mysqlx::Session mysql_ss("localhost", 33060, "svandex", "y1ban@Hust");
        /*
    std::list<mysqlx::Schema> schemaList = mysql_ss.getSchemas();

    for (mysqlx::Schema sch : schemaList)
    {
        std::cout << sch.getName() << std::endl;
    }
    */

        std::string wt = "true";
        if (std::strcmp(d["sex"].GetString(), "female") == 0)
        {
            wt = "false";
        }

        std::string spc_str = "";
        auto num = d["speciality"].Size();
        std::cout<<num<<std::endl;
        for (int i = 0; i < num; i++)
        {
            if (i < num-1)
            {
                spc_str = spc_str + d["speciality"][i].GetString() + ",";
            }
            else
            {
                spc_str = spc_str + d["speciality"][i].GetString();
            }
        }

        std::string sql_stm = "insert into staffmgr(name,password,email,gender,speciality) values('" + std::string(d["name"].GetString()) + "','" + std::string(d["password"].GetString()) + "','" + std::string(d["email"].GetString()) + "'," + wt + ",'" + spc_str + "');";

        std::cout << sql_stm << std::endl;

        mysql_ss.sql("use funtestdemo;").execute();

        auto rsets = mysql_ss.sql(sql_stm.c_str()).execute();
        //std::wcout << rsets.getWarning(0).getMessage().c_str();
        if (rsets.fetchOne().isNull())
        {
            s->send(hdl, "{\"mysql\":\"success\"}", websocketpp::frame::opcode::TEXT);
        }
    }
    catch (mysqlx::Error const &e)
    {
        std::cout << "mysql error: " << std::endl
                  << e.what() << std::endl;
        s->send(hdl, "{\"mysql\":\"error\"}", websocketpp::frame::opcode::TEXT);
    }
    /*
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
    */
}
catch (websocketpp::exception const &e)
{
    std::cout << "websocketpp error:" << std::endl
              << e.what() << std::endl;
}

int main(int argc, char *argv[])
{
    try
    {
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
    }
    catch (websocketpp::exception &e)
    {
        std::wcout << e.what() << std::endl;
    }
}
