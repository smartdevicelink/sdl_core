cmake_minimum_required(VERSION 2.8)
project (ProtocolHandler) 

set(LIBRARIES
        bluetooth)

set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)
include_directories (./include/ ./utils/misc ./src ./ ./transport/bt) 

add_library(ProtocolHandler ./src/BluetoothReader.cpp ./src/BluetoothWriter.cpp ./src/Message.cpp ./src/ProtocolHandler.cpp ./src/MessageGenerator/CMessage.cpp ../../appMain/CBTAdapter.cpp)

install (TARGETS ProtocolHandler DESTINATION bin) 
