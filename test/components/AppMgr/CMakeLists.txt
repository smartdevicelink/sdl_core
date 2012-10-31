cmake_minimum_required(VERSION 2.8)
set (PROJECT testAppManager)
project (${PROJECT})

set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)

link_directories(../../../src/components/AppMgr
				 ../../../src/thirdPartyLibs/jsoncpp
				 ../../../src/components/JSONHandler
				 ../../../src/thirdPartyLibs/axisCore
				 ../../../src/thirdPartyLibs/MessageBroker
				 ../../../src/thirdPartyLibs/logger/bin
				)

include_directories (../../../src/components/AppMgr/include/
					 ../../../src/components/JSONHandler/include/
					 ../../../src/thirdPartyLibs/MessageBroker/include/
					 ../../../src/thirdPartyLibs/jsoncpp/include/
					)

set (SOURCES
		main.cpp
	)

set(LIBRARIES
		appManager
		jsonHandler
		ProtocolHandler
		messagebroker
		messagebrokerclient
		messagebrokerserver
		jsonCpp
		bluetooth
		pthread
		log4cplus
		rt
	)

add_executable(${PROJECT} ${SOURCES})

target_link_libraries(${PROJECT} ${LIBRARIES})

include(ExternalProject)

ExternalProject_Add(
   appManager
   SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/components/AppMgr
   DOWNLOAD_COMMAND ""
   CMAKE_COMMAND cmake
   BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/../../../src/components/AppMgr
   BUILD_COMMAND make
   INSTALL_DIR ../../../src/components/AppMgr/bin
   INSTALL_COMMAND make install
)

ExternalProject_Add(
	jsonHandler
	SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/../JSONHandler/
	DOWNLOAD_COMMAND ""
	CMAKE_COMMAND cmake
	BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/../JSONHandler/
	BUILD_COMMAND make
	INSTALL_DIR ../JSONHandler/bin
	INSTALL_COMMAND make install
)

ADD_DEPENDENCIES(${PROJECT}
					appManager
					jsonHandler
				)

install (TARGETS ${PROJECT} DESTINATION bin)
