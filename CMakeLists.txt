cmake_minimum_required(VERSION 3.15)
project(Boost_Echo_Client)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(Boost_Echo_Client  include/connectionHandler.h src/connectionHandler.cpp src/user.cpp include/user.h src/Book.cpp include/Book.h src/keyboard.cpp include/keyboard.h src/mainclient.cpp src/readsocket.cpp include/mainclient.h include/readsocket.h)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

find_package(Boost 1.65.1 REQUIRED COMPONENTS
        date_time filesystem iostreams system)
find_package(Threads)
target_link_libraries(Boost_Echo_Client Boost::date_time Boost::filesystem
        Boost::iostreams Boost::system)