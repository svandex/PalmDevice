cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /std:c++14")
project(ecd)
link_directories("C:/Users/svandex/Documents/GitHub/vcpkg/installed/x64-windows/lib/")
include_directories(
    "C:/Users/svandex/Documents/GitHub/vcpkg/installed/x64-windows/include/"
    "C:/Users/svandex/Documents/GitHub/websocketpp/"
    "C:/Users/svandex/Documents/GitHub/fast-cpp-csv-parser"
    "C:/Users/svandex/Desktop/ExcelCoding"
    )
add_executable(ecd main.cpp supfun.cpp)
target_link_libraries(ecd boost_system-vc140-mt xlnt)