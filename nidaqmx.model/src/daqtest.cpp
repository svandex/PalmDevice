#include <NIDAQmx.h>

#include <string>
#include <iostream>

int main()
{
    //std::string deviceName = "cDAQ9191-1CF6E75Mod1";
    //std::string deviceName = "cDAQ9191-1D44868Mod1";
    char dname[100];
    char errBuf[2048] = {'\0'};
    int32 status = DAQmxAddNetworkDevice("10.42.0.48","",true,30,dname,100);
    if(status !=0){
        //if (DAQmxGetErrorString(status, errBuf, 2048)== 0)
        if (DAQmxGetExtendedErrorInfo(errBuf, 2048) == 0)
        {
            std::cout << "Status: " << errBuf << std::endl;
        }
        std::cout << "Cannot Add Network device" << std::endl;
        return 0;
    }
    std::string deviceName(dname);
    status = DAQmxSelfTestDevice(deviceName.c_str());
//    int32 status = DAQmxSelfTestDevice(deviceName.c_str());
    if(status !=0){
        //if (DAQmxGetErrorString(status, errBuf, 2048)== 0)
        if (DAQmxGetExtendedErrorInfo(errBuf, 2048) == 0)
        {
            std::cout << "Status: " << errBuf << std::endl;
        }
        std::cout << "Cannot Self Test device" << std::endl;
        return 0;
    }
    status =  DAQmxResetDevice(deviceName.c_str());
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
    std::cout<<"Device Has been founded."<<std::endl;
    return 0;
}