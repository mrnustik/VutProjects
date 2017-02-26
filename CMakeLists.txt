cmake_minimum_required(VERSION 3.6)
project(ftrest)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES_CLIENT
        client.c
        src/ src/operation.h src/operation.c src/memory.c src/memory.h src/error.c src/error.h)

set(SOURCE_FILES_SERVER
        server.c
        src/ src/operation.h src/operation.c src/memory.c src/memory.h src/error.c src/error.h)

add_executable(tfrestd ${SOURCE_FILES_SERVER})

add_executable(tfrest ${SOURCE_FILES_CLIENT})