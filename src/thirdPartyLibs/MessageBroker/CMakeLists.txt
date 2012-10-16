cmake_minimum_required(VERSION 2.8)
set (PROJECT messageBroker)
project (${PROJECT}) 

set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)

include_directories (./include/
                     ../jsoncpp/include
                    ) 

include(ExternalProject)
ExternalProject_Add(
   jsonCpp
   SOURCE_DIR  ../jsoncpp/
   DOWNLOAD_COMMAND ""
   CMAKE_COMMAND cmake
   BUILD_IN_SOURCE 1   
   BUILD_COMMAND make
   INSTALL_DIR ../jsoncpp/bin
   INSTALL_COMMAND make install
)


add_library(messagebroker
                ./src/lib_messagebroker/CMessageBroker.cpp
                ./src/lib_messagebroker/CMessageBrokerRegistry.cpp
                ./src/lib_messagebroker/md5.cpp
                ./src/lib_messagebroker/system.cpp
                ./src/lib_messagebroker/websocket_handler.cpp
           )

add_library(messagebrokerclient
                ./src/client/mb_client.cpp
                ./src/client/mb_tcpclient.cpp
                ./src/client/mb_controller.cpp
           )

add_library(messagebrokerserver
                ./src/server/mb_server.cpp
                ./src/server/mb_tcpserver.cpp
                ./src/server/networking.cpp
            )

install (TARGETS messagebroker messagebrokerclient messagebrokerserver DESTINATION bin) 

