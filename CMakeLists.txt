cmake_minimum_required(VERSION 3.0.0)
project(pd)
# company pc
include_directories("D:\\IncludeFiles\\usrInclude"
					"D:\\IncludeFiles\\usrInclude\\arm-linux-gnueabihf"
					"D:\\IncludeFiles\\usrLocalInclude")

# home pc
include_directories("C:\\Users\\svandex\\Documents\\IncludeFiles\\usrInclude\\include"
					"C:\\Users\\svandex\\Documents\\IncludeFiles\\usrInclude\\include\\arm-linux-gnueabihf"
					"C:\\Users\\svandex\\Documents\\IncludeFiles\\usrLocalInclude"
					"C:\\Users\\svandex\\Documents\\IncludeFiles\\usrInclude\\include\\c++\\5")


# surface pro 4
include_directories("C:\\Users\\svandex\\Documents\\IncludeFiles\\usrInclude"
					"C:\\Users\\svandex\\Documents\\IncludeFiles\\usrInclude\\arm-linux-gnueabihf"
					"C:\\Users\\svandex\\Documents\\IncludeFiles\\usrLocalInclude")


#set(CMAKE_CXX_FLAGS "/EHsc /std:c++latest /await")
set(CMAKE_CXX_FLAGS "-std=c++11 -pthread -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#add the executable
add_executable(tvpd tvUSB.cpp tvGPIO.cpp tvHTTP.cpp tvWS.cpp main.cpp )
target_link_libraries(tvpd cpprest boost_system ssl crypto pigpio rt)
