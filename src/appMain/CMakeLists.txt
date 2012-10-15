cmake_minimum_required(VERSION 2.8)

set (PROJECT appLinkCore)
project (${PROJECT}) 

set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)

link_directories(../thirdPartyLibs/jsoncpp
                 ../components/JSONHandler/bin
                 ../thirdPartyLibs/axisCore/bin
                 ../thirdPartyLibs/jsoncpp/bin
                 )

set(LIBRARIES
        bluetooth
        ProtocolHandler
        jsonHandler
        jsonCpp
    )

include_directories (./
                     ../thirdPartyLibs/axisCore/utils/misc
                     ../thirdPartyLibs/axisCore/include
                     ../thirdPartyLibs/axisCore/src
                     ../thirdPartyLibs/axisCore/
                     ../thirdPartyLibs/axisCore/transport/bt
                     ../components/JSONHandler/include
                     ../thirdPartyLibs/jsoncpp/include/
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
   SOURCE_DIR ../thirdPartyLibs/axisCore/
   DOWNLOAD_COMMAND ""
   CMAKE_COMMAND cmake
   BUILD_IN_SOURCE 1   
   BUILD_COMMAND make
   INSTALL_DIR ../thirdPartyLibs/axisCore/bin
   INSTALL_COMMAND make install
)

ExternalProject_Add(
   jsonHandler
   SOURCE_DIR ../components/JSONHandler
   DOWNLOAD_COMMAND ""
   CMAKE_COMMAND cmake
   BUILD_IN_SOURCE 1 
   BUILD_COMMAND make
   INSTALL_DIR ../components/JSONHandler/bin
   INSTALL_COMMAND make install
)
ADD_DEPENDENCIES(${PROJECT}
                        jsonCpp
                        ProtocolHandler
                        jsonHandler
                        )

install (TARGETS ${PROJECT} DESTINATION bin)
