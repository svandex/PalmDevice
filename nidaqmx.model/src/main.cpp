#include <iostream>
#include <cstdio>
#include <fstream>
#include <functional>
#include <string>
#include <vector>
#include <exception>
#include <thread>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <NIDAQmx.h>

typedef websocketpp::server<websocketpp::config::asio> server;
using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

std::string rootpath;
//std::string rootpath = "C:/Users/svandex/Desktop/3dExhib/";
//std::string rootpath = "C:/Users/svandex/Documents/GitHub/";

//NI setting
const char *tkName = "TVRIG";
std::string deviceName = "cDAQ9191-1CF6E75Mod1";
char errBuf[2048] = {'\0'};

//Channel Name
const char *Ch0Name = "Ch0Chan";
const char *Ch1Name = "Ch1Chan";
const char *Ch2Name = "Ch2Chan";
const char *Ch3Name = "Ch3Chan";

const uint32_t NUM = 100;

//Data
float64 chan0data[NUM];
float64 chan1data[NUM];
float64 chan2data[NUM];
float64 chan3data[NUM];
float64 frontDoorDegree = 0;

//NI Task Handle
TaskHandle tk = 0;

//NI FUNC setting
#define DAQmxErrChk(functionCall)            \
    if (DAQmxFailed(error = (functionCall))) \
        goto Error;                          \
    else

int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

//send_file_to_client
template <typename encoding>
std::string send_file_to_client(server::connection_ptr &con, const std::string &resURI, const std::string &content_type)
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

void on_message(server *s, websocketpp::connection_hdl hdl, server::message_ptr msg)
{
    //front door degree
    float64 sum = 0;
    for(uint8_t tx=0;tx<20;tx++){
        sum = sum+(chan0data[tx]-2.5)*0.00125;
    }
    if(abs(sum)>0.05){
        frontDoorDegree +=sum;
    }
    if(frontDoorDegree<0){frontDoorDegree=0;}
    if(frontDoorDegree>1){frontDoorDegree=1;}

    //RapidJSON
    rapidjson::StringBuffer jsons;
    rapidjson::Writer<rapidjson::StringBuffer> writer(jsons);
    //std::cout<<"PAYLOAD: "<<msg->get_payload()<<std::endl;
    writer.StartObject();
    //"ch0" gyrometer
    //"ch1" force
    //"ch2" temperature
    //"ch3" movement

    //"ch0"
    writer.Key("ch0");
    writer.Double(chan0data[0]);
    /*
    writer.StartArray();
    for (uint8_t tIndex = 0; tIndex < 20; tIndex++)
    {
        writer.Double(chan0data[tIndex*5]);
    }
    writer.EndArray();
    */

    //"ch1"
    writer.Key("ch1");
    writer.Double(chan1data[0]);
   /* 
    writer.StartArray();
    for (uint8_t tIndex = 0; tIndex < 20; tIndex++)
    {
        writer.Double(chan1data[tIndex*5]);
    }
    writer.EndArray();
    */

    //"ch2"
    writer.Key("ch2");
    writer.Double(chan2data[0]);
    writer.Key("ch3");
    writer.Double(chan3data[0]);
    writer.Key("fdd");
    writer.Double(frontDoorDegree);
    writer.EndObject();

    //std::cout << jsons.GetString() << std::endl;
    s->send(hdl, std::string(jsons.GetString()), websocketpp::frame::opcode::TEXT);
    //std::cout<<"Data Send"<<std::endl;
}

void on_http(server *s, websocketpp::connection_hdl hdl)
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

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Parameter Error, Two Arguments Required" << std::endl;
    }
    rootpath = argv[1];
    /*
    std::thread testThread([] {
        while (1)
        {
            std::cout << "ai0"
                      << "\t"
                      << "ai3"<<"\t"<<"diff" << std::endl;
            std::cout << chan0data[0] << "\t" << chan3data[0]<<"\t"<<chan3data[0]-chan0data[0] << std::endl;
            std::cout << chan0data[1] << "\t" << chan3data[1]<<"\t" <<chan3data[1]-chan0data[1]<< std::endl;
            std::cout << chan0data[2] << "\t" << chan3data[2]<<"\t" <<chan3data[2]-chan0data[2]<< std::endl;
            std::cout << chan0data[3] << "\t" << chan3data[3]<<"\t" <<chan3data[3]-chan0data[3]<< std::endl;
            std::cout << chan0data[4] << "\t" << chan3data[4]<<"\t" <<chan3data[4]-chan0data[4]<< std::endl;
            system("cls");
        }
    });
    testThread.detach();
    */

    //
    int32 error = 0;
    char errBuff[2048] = {'\0'};

    //server setting
    server tvrig_server;
    tvrig_server.set_open_handshake_timeout(1000000);
    tvrig_server.set_max_http_body_size(64000000);

    tvrig_server.set_error_channels(websocketpp::log::elevel::fatal);
    //tvrig_server.set_access_channels(websocketpp::log::alevel::app); // ^ websocketpp::log::alevel::frame_payload);

    tvrig_server.set_http_handler(bind(&on_http, &tvrig_server, ::_1));
    tvrig_server.set_message_handler(bind(&on_message, &tvrig_server, ::_1, ::_2));

    tvrig_server.init_asio();
    tvrig_server.listen(websocketpp::lib::asio::ip::tcp::v4(), 9002);
    tvrig_server.start_accept();

    //reset device
    int32 status = DAQmxResetDevice(deviceName.c_str());
    //
    if (status != 0)
    {
        //if (DAQmxGetErrorString(status, errBuf, 2048)== 0)
        if (DAQmxGetExtendedErrorInfo(errBuf, 2048) == 0)
        {
            std::cout << "Status: " << errBuf << std::endl;
        }
        std::cout << "Cannot reset device" << std::endl;
        return 0;
    }

    std::cout << "Server Start..." << std::endl;

    //---------------------------------------------------
    //Distance Measurement Setting
    DAQmxErrChk(DAQmxCreateTask(tkName, &tk));

    DAQmxErrChk(DAQmxCreateAIVoltageChan(tk, (deviceName + "/ai0").c_str(), Ch0Name, DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL));
    DAQmxErrChk(DAQmxCreateAIVoltageChan(tk, (deviceName + "/ai1").c_str(), Ch1Name, DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL));
    DAQmxErrChk(DAQmxCreateAIVoltageChan(tk, (deviceName + "/ai2").c_str(), Ch2Name, DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL));
    DAQmxErrChk(DAQmxCreateAIVoltageChan(tk, (deviceName + "/ai3").c_str(), Ch3Name, DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL));

    DAQmxErrChk(DAQmxCfgSampClkTiming(tk, "", 2000.0, DAQmx_Val_Rising, DAQmx_Val_ContSamps, NUM));

    //CALLBACK FUNCTION
    DAQmxErrChk(DAQmxRegisterEveryNSamplesEvent(tk, DAQmx_Val_Acquired_Into_Buffer, NUM, 0, EveryNCallback, NULL));
    DAQmxErrChk(DAQmxRegisterDoneEvent(tk, 0, DoneCallback, NULL));

    //Start
    DAQmxErrChk(DAQmxStartTask(tk));

    tvrig_server.run();

Error:
    if (DAQmxFailed(error))
        DAQmxGetExtendedErrorInfo(errBuff, 2048);
    if (tk != 0)
    {
        /*********************************************/
        // DAQmx Stop Code
        /*********************************************/
        DAQmxStopTask(tk);
        DAQmxClearTask(tk);
        tk = 0;
    }
    if (DAQmxFailed(error))
        printf("DAQmx Error: %s\n", errBuff);
    printf("End of program, press Enter key to quit\n");
    getchar();
    return 0;
}

int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
    int32 error = 0;
    char errBuff[2048] = {'\0'};
    int32 read;
    float64 data[4 * NUM];

    /*********************************************/
    // DAQmx Read Code
    /*********************************************/

    DAQmxErrChk(DAQmxReadAnalogF64(tk, NUM, 10.0, DAQmx_Val_GroupByChannel, data, NUM * 8, &read, NULL));

    memcpy(chan0data, data, NUM * 8);
    memcpy(chan1data, data + NUM, NUM * 8);
    memcpy(chan2data, data + NUM * 2, NUM * 8);
    memcpy(chan3data, data + NUM * 3, NUM * 8);

Error:
    if (DAQmxFailed(error))
    {
        DAQmxGetExtendedErrorInfo(errBuff, 2048);
        if (tk != 0)
        {
            /*********************************************/
            // DAQmx Stop Code
            /*********************************************/
            DAQmxStopTask(tk);
            DAQmxClearTask(tk);
            tk = 0;
        }
        printf("DAQmx Error: %s\n", errBuff);
    }
    return 0;
}

int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData)
{
    int32 error = 0;
    char errBuff[2048] = {'\0'};

    // Check to see if an error stopped the task.
    DAQmxErrChk(status);

Error:
    if (DAQmxFailed(error))
    {
        DAQmxGetExtendedErrorInfo(errBuff, 2048);
        DAQmxClearTask(taskHandle);
        printf("DAQmx Error: %s\n", errBuff);
    }
    return 0;
}
