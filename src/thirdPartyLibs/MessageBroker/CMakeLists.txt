cmake_minimum_required(VERSION 2.8)
set (PROJECT messageBroker)
project (${PROJECT}) 

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)

include_directories (
                     ./include/
                     ../jsoncpp/include
                    ) 

link_directories(
                 ../thirdPartyLibs/jsoncpp/bin
                )

set(LIBRARIES
        jsonCpp
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

## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-O0 -g3 -ggdb3")        ## DON'T Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-m")  ## Strip binary
endif()

install (TARGETS messagebroker messagebrokerclient messagebrokerserver DESTINATION bin) 

