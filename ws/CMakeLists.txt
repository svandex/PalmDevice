cmake_minimum_required(VERSION 3.9)
project(wstest)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(~/Desktop/ngx_html/dev/websocketpp
 ~/Desktop/ngx_html/dev//rapidjson/include)

link_directories(/usr/lib/x86_64-linux-gnu/)

add_executable(wstest test.cpp)
target_link_libraries(wstest boost_system)
