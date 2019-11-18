cmake_minimum_required(VERSION 2.7)
project(client)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -lpthread -pthread")

add_executable(client main.cpp client.h client.cpp)