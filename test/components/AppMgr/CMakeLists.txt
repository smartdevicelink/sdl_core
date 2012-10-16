cmake_minimum_required(VERSION 2.8)
project (testAppManager)

link_directories(../../../src/components/AppMgr ../../../src/thirdPartyLibs/jsoncpp ../../../src/components/JSONHandler ../../../src/thirdPartyLibs/axisCore/bin ../../../src/thirdPartyLibs/MessageBroker)

include_directories (../../../src/components/AppMgr/include/ ../../../src/components/JSONHandler/include/ ../../../src/thirdPartyLibs/MessageBroker/include/  ../../../src/thirdPartyLibs/jsoncpp/include/)
add_executable(testAppManager main.cpp)
target_link_libraries(testAppManager appManager jsonHandler jsonCpp ProtocolHandler messagebroker messagebrokerclient messagebrokerserver bluetooth pthread)

include(ExternalProject)
ExternalProject_Add(
   appManager
   SOURCE_DIR ../../../src/components/AppMgr
   DOWNLOAD_COMMAND ""
   CMAKE_COMMAND cmake
   BUILD_IN_SOURCE 1 
   BUILD_COMMAND make
   INSTALL_DIR ../../../src/components/AppMgr/bin
   INSTALL_COMMAND make install
)

ExternalProject_Add(
	jsonHandler
	SOURCE_DIR  ../JSONHandler/
	DOWNLOAD_COMMAND ""
	CMAKE_COMMAND cmake
	BUILD_IN_SOURCE 1
	BUILD_COMMAND make
	INSTALL_DIR ../JSONHandler/bin
	INSTALL_COMMAND make install
)

ADD_DEPENDENCIES(testAppManager appManager)

set(CMAKE_INSTALL_PREFIX .)
install (TARGETS testAppManager DESTINATION bin)
