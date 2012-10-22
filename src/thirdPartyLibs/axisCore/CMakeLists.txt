cmake_minimum_required(VERSION 2.8)
project (ProtocolHandler) 

set(LIBRARIES
        bluetooth
        pthread
        log4cplus)

set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)
include_directories (./include/ 
                     ./utils/misc 
                     ./src 
                     ./ 
                     ./transport/bt 
                     ../logger/include
                     ../logger/log4cplus-1.0.4/include) 

add_library(ProtocolHandler 
            ./src/BluetoothReader.cpp 
            ./src/BluetoothWriter.cpp 
            ./src/Message.cpp 
            ./src/ProtocolHandler.cpp 
            ./src/MessageGenerator/CMessage.cpp 
            ../../appMain/CBTAdapter.cpp)

install (TARGETS ProtocolHandler DESTINATION bin) 
