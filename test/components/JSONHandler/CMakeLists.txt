cmake_minimum_required(VERSION 2.8)
project (TESTJSONHANDLER) 

set(CMAKE_MODULE_PATH ../../../src/thirdPartyLibs/jsoncpp)
find_package(JSONCPP)
#add_binarydiirectory(../../../src/thirdPartyLibs/jsoncpp/)
#add_subdirectory(../../../src/components/JSONHandler)
#add_dependencies(testJsonHandler JSONCPP)
link_directories(../../../src/thirdPartyLibs/jsoncpp ../../../src/components/JSONHandler)

include_directories (../../../src/components/JSONHandler/include/ ../../../src/thirdPartyLibs/jsoncpp/include/) 
add_executable(testJsonHandler main.cpp)
target_link_libraries(testJsonHandler jsonCpp jsonHandler)
 


