cmake_minimum_required(VERSION 3.0.0)
project(pd)

#set(CMAKE_CXX_FLAGS "/EHsc /std:c++latest /await")
set(CMAKE_CXX_FLAGS "-std=c++11 -pthread -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#add the executable
add_executable(tvpd tvUSB.cpp tvGPIO.cpp tvHTTP.cpp tvWS.cpp main.cpp )
target_link_libraries(tvpd cpprest boost_system ssl crypto pigpio rt)
