cmake_minimum_required(VERSION 3.0.0)
project(pd)
<<<<<<< HEAD
# company pc
include_directories("D:\\IncludeFiles\\usrInclude"
					"D:\\IncludeFiles\\usrInclude\\arm-linux-gnueabihf"
					"D:\\IncludeFiles\\usrLocalInclude")

# home pc
include_directories("C:\\Users\\svandex\\Documents\\IncludeFiles\\usrInclude\\include\\"
					"C:\\Users\\svandex\\Documents\\IncludeFiles\\usrInclude\\include\\arm-linux-gnueabihf\\"
					"C:\\Users\\svandex\\Documents\\IncludeFiles\\usrLocalInclude\\")


# surface pro 4
include_directories("C:\\Users\\svandex\\Documents\\IncludeFiles\\usrInclude"
					"C:\\Users\\svandex\\Documents\\IncludeFiles\\usrInclude\\arm-linux-gnueabihf"
					"C:\\Users\\svandex\\Documents\\IncludeFiles\\usrLocalInclude")

=======
>>>>>>> a9cf27ef14e11915257921b527ef1355871e4644

#set(CMAKE_CXX_FLAGS "/EHsc /std:c++latest /await")
set(CMAKE_CXX_FLAGS "-std=c++11 -pthread -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#add the executable
add_executable(tvpd tvGPIO.cpp tvHTTP.cpp tvWS.cpp main.cpp )
<<<<<<< HEAD
target_link_libraries(tvpd cpprest boost_system ssl crypto pigpio rt usb-1.0)
=======
target_link_libraries(tvpd cpprest boost_system ssl crypto pigpio rt)
>>>>>>> a9cf27ef14e11915257921b527ef1355871e4644
