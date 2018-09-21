cmake_minimum_required(VERSION 3.9)
project(wstest)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR}
${PROJECT_SOURCE_DIR}/rapidjson/include
${PROJECT_SOURCE_DIR}/websocketpp
/home/svandex/Documents/mysql-connector-c++/include)

link_directories(/usr/lib/x86_64-linux-gnu/
/home/svandex/Documents/mysql-connector-c++/lib64)

add_executable(wstest test.cpp)
target_link_libraries(wstest boost_system mysqlcppconn8)
