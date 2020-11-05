cmake_minimum_required(VERSION 3.15)
project(ex1)
include_directories(include/rapidjson)

set(CMAKE_CXX_STANDARD 20)

add_executable(ex1 main.cpp)