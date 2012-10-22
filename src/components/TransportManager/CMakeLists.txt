cmake_minimum_required(VERSION 2.8)
set (PROJECT TransportManager)
project (${PROJECT})

set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)

include_directories(./include
                    ../../thirdPartyLibs/logger/include/)

set (SOURCES ./src/IDeviceAdapter.cpp
             ./src/ITransportManager.cpp
             ./src/ITransportManagerListener.cpp
             ./src/CTransportManager.cpp)

add_library(${PROJECT} ${SOURCES})

install (TARGETS ${PROJECT} DESTINATION bin)

