cmake_minimum_required(VERSION 2.8)
project (JSONHANDLER) 

include_directories (./include/ ../../thirdPartyLibs/jsoncpp/include/) 
add_library(jsonHandler ./src/JSONHandler.cpp ./src/MobileRPCMessage.cpp ./src/MobileRPCRequest.cpp ./src/MobileRPCResponse.cpp)
 
export(PACKAGE JSONHANDLER)
#file(RELATIVE_PATH CONF_REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"   "${INSTALL_INCLUDE_DIR}")
configure_file(JSONHANDLERConfig.cmake.in   "${PROJECT_BINARY_DIR}/JSONHANDLERConfig.cmake" @ONLY)


