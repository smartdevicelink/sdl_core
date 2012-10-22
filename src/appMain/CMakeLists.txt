cmake_minimum_required(VERSION 2.8)

set (PROJECT appLinkCore)
project (${PROJECT})

set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)

link_directories(../thirdPartyLibs/jsoncpp
                 ../components/JSONHandler/bin
                 ../components/AppMgr/bin
                 ../thirdPartyLibs/axisCore/bin
                 ../thirdPartyLibs/jsoncpp/bin
                 ../thirdPartyLibs/MessageBroker/bin
                 ../thirdPartyLibs/logger/bin
                 )

set(LIBRARIES
        appManager
        jsonHandler
        ProtocolHandler
        messagebrokerclient
        messagebrokerserver
        messagebroker
        jsonCpp
        bluetooth
        pthread
        log4cplus
    )

include_directories (./
                     ../thirdPartyLibs/axisCore/utils/misc
                     ../thirdPartyLibs/axisCore/include
                     ../thirdPartyLibs/axisCore/src
                     ../thirdPartyLibs/axisCore/
                     ../thirdPartyLibs/axisCore/transport/bt
                     ../thirdPartyLibs/jsoncpp/include/
                     ../components/JSONHandler/include
                     ../components/AppMgr/include
                     ../thirdPartyLibs/MessageBroker/include
                     ../thirdPartyLibs/logger/include
                     ../thirdPartyLibs/logger/log4cplus-1.0.4/include
                     )

set (SOURCES
        appMain.cpp
        CBTAdapter.cpp
    )

add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} ${LIBRARIES})


include(ExternalProject)

ExternalProject_Add(
   ProtocolHandler
   SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../thirdPartyLibs/axisCore/
   DOWNLOAD_COMMAND ""
   CMAKE_COMMAND cmake
   BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/../thirdPartyLibs/axisCore/
   BUILD_COMMAND make
   INSTALL_DIR ../thirdPartyLibs/axisCore/bin
   INSTALL_COMMAND make install
)

ExternalProject_Add(
   jsonCpp
   SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../thirdPartyLibs/jsoncpp
   DOWNLOAD_COMMAND ""
   CMAKE_COMMAND cmake
   BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/../thirdPartyLibs/jsoncpp
   BUILD_COMMAND make
   INSTALL_DIR ../thirdPartyLibs/jsoncpp/bin
   INSTALL_COMMAND make install
)

ExternalProject_Add(
   jsonHandler
   SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../components/JSONHandler
   DOWNLOAD_COMMAND ""
   CMAKE_COMMAND cmake
   BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/../components/JSONHandler
   BUILD_COMMAND make
   INSTALL_DIR ../components/JSONHandler/bin
   INSTALL_COMMAND make install
)

ExternalProject_Add(
   messagebroker
   messagebrokerclient
   messagebrokerserver
   SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../thirdPartyLibs/MessageBroker
   DOWNLOAD_COMMAND ""
   CMAKE_COMMAND cmake
   BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/../thirdPartyLibs/MessageBroker
   BUILD_COMMAND make
   INSTALL_DIR ../thirdPartyLibs/MessageBroker/bin
   INSTALL_COMMAND make install
)

ExternalProject_Add(
   appManager
   SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../components/AppMgr
   DOWNLOAD_COMMAND ""
   CMAKE_COMMAND cmake
   BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/../components/AppMgr
   BUILD_COMMAND make
   INSTALL_DIR ../components/AppMgr/bin
   INSTALL_COMMAND make install
)

ADD_DEPENDENCIES(${PROJECT}
                        log4cplus
                        jsonCpp
                        messagebroker
                        messagebrokerserver
                        messagebrokerclient
                        ProtocolHandler
                        jsonHandler
                        appManager
                        )

install (TARGETS ${PROJECT} DESTINATION bin)
