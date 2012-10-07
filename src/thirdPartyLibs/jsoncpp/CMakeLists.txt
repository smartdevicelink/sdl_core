cmake_minimum_required(VERSION 2.8)
project (jsonCpp) 

set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)
include_directories (./include/) 
add_library(jsonCpp ./src/json_value.cpp ./src/json_writer.cpp ./src/json_reader.cpp)

install (TARGETS jsonCpp DESTINATION bin) 

