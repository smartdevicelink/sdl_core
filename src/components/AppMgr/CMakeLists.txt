cmake_minimum_required(VERSION 2.8)
set (PROJECT appManager)
project (${PROJECT})

set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)

include_directories (./include/
					 ../JSONHandler/include/
					 ../../thirdPartyLibs/MessageBroker/include/
					 ../../thirdPartyLibs/jsoncpp/include/
					)

set (SOURCES
		./src/AppMgr.cpp
		./src/AppFactory.cpp
		./src/AppLinkInterface.cpp
		./src/AppMgrRegistry.cpp
		./src/IApplication.cpp
		./src/RPCAppLinkFactory.cpp
		./src/RPCBusFactory.cpp
		./src/Application.cpp
		./src/AppMgrCore.cpp
		./src/AppPolicy.cpp
		./src/RegistryItem.cpp
		./src/RPCAppLinkObject.cpp
		./src/RPCBusObject.cpp
	)

add_library(${PROJECT} ${SOURCES})

install (TARGETS ${PROJECT} DESTINATION bin)
