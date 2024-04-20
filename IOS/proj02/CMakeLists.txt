cmake_minimum_required(VERSION 2.8.4)
project(proj02)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

add_custom_target(mytest COMMAND make -C ${proj02_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})