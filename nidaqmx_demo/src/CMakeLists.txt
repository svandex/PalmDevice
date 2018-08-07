cmake_minimum_required(VERSION 3.9)
project(exhib)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(/usr/svandex/repositories/websocketpp)

link_directories(/usr/lib/x86_64-linux-gnu/)

add_executable(exhib main.cpp)
target_link_libraries(exhib boost_system nidaqmx)

add_executable(daqtest daqtest.cpp)
target_link_libraries(daqtest nidaqmx)
