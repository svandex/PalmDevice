#include <supfun.hpp>

//send_file_to_client
template <typename encoding>
std::string juncheng::send_file_to_client(server::connection_ptr &con, const std::string &resURI, const std::string &content_type)
{
    //return variable
    std::string response;
    //filepath
    std::string filepath = rootpath + resURI;
    //std::basic_fstream<encoding> file;
    //file.open(filepath.c_str(), std::ios::in|std::ios::out|std::ios::binary);
    std::FILE *file = std::fopen(filepath.c_str(), "rb");
    //file.open(filepath.c_str(), std::ios::in | std::ios::out);
    if (!file)
    {
        // 404 error
        std::stringstream ss;
        ss << "<!doctype html><html><head>"
           << "<title>Error 404 (Resource not found)</title><body>"
           << "<h1>Error 404</h1>"
           << "<p>The requested URL " << resURI << " was not found on this server.</p>"
           << "</body></head></html>";
        con->set_status(websocketpp::http::status_code::not_found);
        return std::move(ss.str());
    }

    con->append_header("Content-Type", content_type);

    //fstream method is slow
    /*
    file.seekg(0, std::ios::end);
	std::cout << "File Size: " << file.tellg();
    response.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    response.assign((std::istreambuf_iterator<encoding>(file)), std::istreambuf_iterator<encoding>());
	*/
    std::fseek(file, 0, SEEK_END);
    uint64_t filesize = std::ftell(file);
    std::fseek(file, 0, SEEK_SET);
    //vector
    std::vector<encoding> buf(filesize);
    std::fread(buf.data(), sizeof encoding, buf.size(), file);

    con->set_status(websocketpp::http::status_code::ok);
    std::fclose(file);

    //resize string
    response.resize(filesize);
    response.assign(buf.begin(), buf.end());
    return std::move(response);
}

void juncheng::on_message(server *s, websocketpp::connection_hdl hdl, server::message_ptr msg)
{
    //RapidJSON
    /*
    rapidjson::StringBuffer jsons;
    rapidjson::Writer<rapidjson::StringBuffer> writer(jsons);

    writer.StartObject();
    writer.EndObject();

    s->send(hdl, std::string(jsons.GetString()), websocketpp::frame::opcode::TEXT);
    */
    rapidjson::Document d;
    d.Parse(msg->get_payload().c_str());
    //excel file generation
    std::cout<<msg->get_payload()<<std::endl;
    uint16_t indexCap = d.Size();

    try
    {
        xlnt::workbook wb;
        xlnt::worksheet ws = wb.active_sheet();
        for (uint16_t index = 0; index < indexCap; index++)
        {
            ws.cell(1,index+ 1).value(d[index]["testno"].GetString());
            ws.cell(2, index+1).value(d[index]["testname"].GetString());
            ws.cell(3, index+1).value(d[index]["testfee"].GetString());
            ws.cell(4, index+1).value(d[index]["delegatefee"].GetString());
            ws.cell(5, index+1).value(d[index]["labourfee"].GetString());
        }
        wb.save("test.xlsx");
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    s->send(hdl, "Excel", websocketpp::frame::opcode::TEXT);
}

void juncheng::on_http(server *s, websocketpp::connection_hdl hdl)
{
    std::string response;
    server::connection_ptr con = s->get_con_from_hdl(hdl);
    //resouce URI
    auto resURI = con->get_uri()->get_resource();

    if (resURI.compare("/") == 0)
    {
        response = send_file_to_client<char>(con, std::string{"/index.html"}, "text/html");
    }
    else
    {
        //send HTML file
        if (resURI.substr(resURI.size() - 3).compare("tml") == 0)
        {
            response = send_file_to_client<char>(con, resURI, "text/html");
        }
        //send JS file
        if (resURI.substr(resURI.size() - 3).compare(".js") == 0)
        {
            response = send_file_to_client<char>(con, resURI, "text/javascript");
        }
        //send STL file
        if (resURI.substr(resURI.size() - 3).compare("stl") == 0)
        {
            response = send_file_to_client<unsigned char>(con, resURI, "application/octet-stream");
        }
        //send logo
        if (resURI.substr(resURI.size() - 3).compare("png") == 0)
        {
            response = send_file_to_client<unsigned char>(con, resURI, "application/x-png");
        }
        //send logo
        if (resURI.substr(resURI.size() - 3).compare("jpg") == 0)
        {
            response = send_file_to_client<char>(con, resURI, "application/x-jpg");
        }
    }
    con->set_body(response);
}