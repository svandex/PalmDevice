cmake_minimum_required(VERSION 3.0.0)
project(pd)
include_directories("C:\\Users\\svandexcn\\Documents\\RPi3_include\\include"
					"C:\\Users\\svandexcn\\Documents\\RPi3_include\\include\\local"
					"C:\\Users\\svandexcn\\Documents\\RPi3_include\\include\\c++\\arm-linux-gnueabihf")


#set(CMAKE_CXX_FLAGS "/EHsc /std:c++latest /await")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#add the executable
add_executable(pd main.cpp PalmDeviceGPIO.cpp PalmDeviceServer.cpp)
target_link_libraries(mlc WindowsApp.lib)# cpprest_2_10.lib)