cmake_minimum_required(VERSION 2.8)
set (PROJECT appManager)
project (${PROJECT})

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)

include_directories (./include/
					 ../JSONHandler/include/
					 ../../thirdPartyLibs/MessageBroker/include/
					 ../../thirdPartyLibs/jsoncpp/include/
					 ../../thirdPartyLibs/axisCore/
					 ../../thirdPartyLibs/axisCore/include/
					 ../../thirdPartyLibs/axisCore/src/
					 ../../thirdPartyLibs/logger/include/
					 ../../thirdPartyLibs/logger/log4cplus-1.0.4/include/
					)

set (SOURCES
		./src/SubscribeButton.cpp
		./src/IAppCommand.cpp
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

## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "-O0 -g3 -ggdb3 -std=c++0x")	## DON'T Optimize
	set(CMAKE_EXE_LINKER_FLAGS "-m")  			  		## DON'T Strip binary
endif()

install (TARGETS ${PROJECT} DESTINATION bin)
