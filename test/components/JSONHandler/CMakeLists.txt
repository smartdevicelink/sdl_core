cmake_minimum_required(VERSION 2.8)
project (testJsonHandler) 

link_directories(../../../src/thirdPartyLibs/jsoncpp ../../../src/components/JSONHandler)

include_directories (../../../src/components/JSONHandler/include/ ../../../src/thirdPartyLibs/jsoncpp/include/) 
add_executable(testJsonHandler main.cpp)
target_link_libraries(testJsonHandler jsonCpp jsonHandler)
 

include(ExternalProject)
ExternalProject_Add(
   jsonHandler
   SOURCE_DIR ../../../src/components/JSONHandler
   DOWNLOAD_COMMAND ""
   CMAKE_COMMAND cmake
   BUILD_IN_SOURCE 1 
#   BINARY_DIR ../../../src/components/JSONHandler/bin  
   BUILD_COMMAND make
   INSTALL_DIR ../../../src/components/JSONHandler/bin
   INSTALL_COMMAND make install
)

ADD_DEPENDENCIES(testJsonHandler jsonHandler)

set(CMAKE_INSTALL_PREFIX .)
install (TARGETS testJsonHandler DESTINATION bin)

