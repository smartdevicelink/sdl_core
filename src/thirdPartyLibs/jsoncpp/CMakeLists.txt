cmake_minimum_required(VERSION 2.8)
project (jsonCpp) 

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)
include_directories (./include/) 
add_library(jsonCpp ./src/json_value.cpp ./src/json_writer.cpp ./src/json_reader.cpp)

## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-O0 -g3 -ggdb3")        ## DON'T Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-m")  ## Strip binary
endif()

install (TARGETS jsonCpp DESTINATION bin) 

