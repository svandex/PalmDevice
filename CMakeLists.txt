cmake_minimum_required(VERSION 3.0.0)
project(pd)
include_directories("D:\\IncludeFiles\\usrInclude"
					"D:\\IncludeFiles\\usrInclude\\arm-linux-gnueabihf"
					"D:\\IncludeFiles\\usrLocalInclude")


#set(CMAKE_CXX_FLAGS "/EHsc /std:c++latest /await")
set(CMAKE_CXX_FLAGS "-std=c++11 -pthread -fpermissive -Wwrite-strings")
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#add the executable
add_executable(pd main.cpp PalmDeviceGPIO.cpp PalmDeviceServer.cpp)
target_link_libraries(pd cpprest boost_system ssl crypto pigpio rt)
