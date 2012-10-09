cmake_minimum_required(VERSION 2.8)
project (ProtocolHandler) 

set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)
include_directories (./include/ ./utils/misc ./src ./) 

add_library(ProtocolHandler ./src/BluetoothReader.cpp ./src/BluetoothWriter.cpp ./src/Message.cpp ./src/ProtocolHandler.cpp)

install (TARGETS ProtocolHandler DESTINATION bin) 
