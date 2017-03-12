cmake_minimum_required(VERSION 3.6)
project(ipk_proj1_cpp)

set(CMAKE_CXX_STANDARD 11)

set(SERVER_SOURCE_FILES server.cpp
        Library/Logger.cpp
        Library/Logger.h
        Library/Memory.cpp
        Library/Memory.h
        Library/Operation.h
        Library/Operation.cpp
        Library/Files.cpp
        Library/Files.h
        Library/Http.cpp
        Library/Http.h Library/Directories.cpp Library/Directories.h)

set(CLIENT_SOURCE_FILES client.cpp
        Library/Logger.cpp
        Library/Logger.h
        Library/Memory.cpp
        Library/Memory.h
        Library/Operation.h
        Library/Operation.cpp
        Library/Files.cpp
        Library/Files.h
        Library/Http.cpp
        Library/Http.h Library/Directories.cpp Library/Directories.h Library/Codes.h)

add_executable(ftrestd ${SERVER_SOURCE_FILES})

add_executable(ftrest ${CLIENT_SOURCE_FILES})