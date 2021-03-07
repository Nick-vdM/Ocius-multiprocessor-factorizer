cmake_minimum_required(VERSION 3.17)
project(Ocius)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(client client.cpp)
add_executable(server server.cpp)
