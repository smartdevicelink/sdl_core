cmake_minimum_required(VERSION 2.8)
project (jsonHandler)

include_directories (./include/
                     ../../thirdPartyLibs/jsoncpp/include/
                     ../../thirdPartyLibs/axisCore/include
                     ../../thirdPartyLibs/axisCore/
                     ../../thirdPartyLibs/axisCore/src
                    )

add_library(jsonHandler
                  ./src/JSONHandler.cpp
                  ./src/MobileRPCMessage.cpp
                  ./src/MobileRPCRequest.cpp
                  ./src/MobileRPCResponse.cpp
                  ./src/MobileRPCNotification.cpp
                  ./src/MobileRPCFactory.cpp
                  ./src/RPC2Command.cpp
                  ./src/RPC2Marshaller.cpp
                  ./src/RPC2Notification.cpp
                  ./src/OnButtonEventMarshaller.cpp
                  ./src/OnButtonEvent.cpp
                  ./src/ButtonEventModeMarshaller.cpp
           )

include(ExternalProject)
ExternalProject_Add(
   jsonCpp
   SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdPartyLibs/jsoncpp/
   DOWNLOAD_COMMAND ""
   CMAKE_COMMAND cmake
   BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/../../thirdPartyLibs/jsoncpp/
   BUILD_COMMAND make
   INSTALL_DIR ../../thirdPartyLibs/jsoncpp/bin
   INSTALL_COMMAND make install
)

ExternalProject_Add(
   ProtocolHandler
   SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdPartyLibs/axisCore/
   DOWNLOAD_COMMAND ""
   CMAKE_COMMAND cmake
   BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/../../thirdPartyLibs/axisCore/
   BUILD_COMMAND make
   INSTALL_DIR ../../thirdPartyLibs/axisCore/bin
   INSTALL_COMMAND make install
)


set(CMAKE_INSTALL_PREFIX .)
install (TARGETS jsonHandler DESTINATION bin)

