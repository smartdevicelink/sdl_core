cmake_minimum_required(VERSION 2.8)
project (testJsonHandler) 

link_directories(../../../src/thirdPartyLibs/jsoncpp ../../../src/components/JSONHandler ../../../src/thirdPartyLibs/axisCore/bin)

include_directories (../../../src/components/JSONHandler/include/ 
			../../../src/thirdPartyLibs/jsoncpp/include/ 
			../../../src/thirdPartyLibs/axisCore/include 
			../../../src/thirdPartyLibs/axisCore/ 
			../../../src/thirdPartyLibs/axisCore/src
			../../../src/appMain
			../../../src/thirdPartyLibs/axisCore/utils/misc
			../../../src/thirdPartyLibs/axisCore/transport/bt)
 
add_executable(testJsonHandler main.cpp)
target_link_libraries(testJsonHandler jsonCpp jsonHandler ProtocolHandler bluetooth)
#CBTAdapter.cpp 

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

