cmake_minimum_required(VERSION 3.8)
project(isamon)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Application.cpp
        Application.h
        Arguments.cpp
        Arguments.h
        ArpScanner.cpp
        ArpScanner.h
        ICMPSender.cpp
        ICMPSender.h
        IpAddress.cpp
        IpAddress.h
        IpNetwork.cpp
        IpNetwork.h
        IpNetworkEnumerator.cpp
        IpNetworkEnumerator.h
        Logger.cpp
        Logger.h
        main.cpp
        NetworkHelper.cpp
        NetworkHelper.h
        ScannerBase.cpp
        ScannerBase.h
        TCPScanner.cpp
        TCPScanner.h
        UdpScanner.cpp
        UdpScanner.h)

add_executable(isamon ${SOURCE_FILES})
target_link_libraries(isamon ${PCAP_LIBRARY})
