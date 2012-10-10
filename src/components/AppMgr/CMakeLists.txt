cmake_minimum_required(VERSION 2.8)
project (appManager) 

include_directories (./include/ ../JSONHandler/include/ ../../thirdPartyLibs/MessageBroker/include/) 
add_library(appManager ./src/AppFactory.cpp ./src/AppLinkInterface.cpp ./src/AppMgrRegistry.cpp ./src/IApplication.cpp ./src/RPCAppLinkFactory.cpp ./src/RPCBusFactory.cpp ./src/Application.cpp src/AppMgrCore.cpp ./src/AppPolicy.cpp ./src/RegistryItem.cpp ./src/RPCAppLinkObject.cpp ./src/RPCBusObject.cpp)

include(ExternalProject)
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

set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)
install (TARGETS appManager DESTINATION bin)
