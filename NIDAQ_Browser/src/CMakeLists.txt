cmake_minimum_required(VERSION 3.9)
project(exhib)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

#include_directories
include_directories("C:\\Users\\svandex\\Documents\\GitHub\\vcpkg\\installed\\x64-windows\\include\\"
"C:\\Users\\svandex\\Documents\\GitHub\\websocketpp\\"
"C:\\Program Files (x86)\\National Instruments\\Shared\\ExternalCompilerSupport\\C\\include"
"C:\\Users\\svandex\\Documents\\GitHub\\websocketpp\\"
"C:\\Users\\svandex\\Documents\\GitHub\\kalman-cpp\\")

#link_directories
link_directories("C:\\Users\\svandex\\Documents\\GitHub\\vcpkg\\installed\\x64-windows\\lib\\"
"C:\\Program Files (x86)\\National Instruments\\Shared\\ExternalCompilerSupport\\C\\lib64\\msvc"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(exhib main.cpp)
target_link_libraries(exhib boost_system-vc140-mt nidaqmx)
#target_link_libraries(exhib boost_system-vc140-mt boost_date_time-vc140-mt boost_chrono-vc140-mt pthreadsVC2 OpenSSL::SSL OpenSSL::Crypto)
