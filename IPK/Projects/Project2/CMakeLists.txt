cmake_minimum_required(VERSION 3.6)
project(ipk_proj_2_client)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Library/Socket.cpp Library/Socket.h)
add_executable(ipk_proj_2_client ${SOURCE_FILES})