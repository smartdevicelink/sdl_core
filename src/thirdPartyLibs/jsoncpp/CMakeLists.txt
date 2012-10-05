cmake_minimum_required(VERSION 2.8)
project (JSONCPP) 

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR . CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include)

include_directories (./include/) 
add_library(jsonCpp ./src/json_value.cpp ./src/json_writer.cpp ./src/json_reader.cpp)
 
export(PACKAGE JSONCPP)
#file(RELATIVE_PATH CONF_REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"   "${INSTALL_INCLUDE_DIR}")
configure_file(JSONCPPConfig.cmake.in   "${PROJECT_BINARY_DIR}/JSONCPPConfig.cmake" @ONLY)
#install(FILES   "${PROJECT_BINARY_DIR}/JSONCppConfig.cmake"   DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
#install(EXPORT FooBarLibraryDepends DESTINATION   "${INSTALL_CMAKE_DIR}" COMPONENT dev)
