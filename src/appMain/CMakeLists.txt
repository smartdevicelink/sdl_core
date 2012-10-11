cmake_minimum_required(VERSION 2.8)

set (PROJECT appMain)
project (${PROJECT}) 

set(CMAKE_INSTALL_PREFIX .)
set(ARCHIVE_OUTPUT_DIRECTORY ./bin)

link_directories(../thirdPartyLibs/jsoncpp
                 ../components/JSONHandler/bin
                 ../thirdPartyLibs/axisCore/bin)

set(LIBRARIES
        bluetooth
        ProtocolHandler)

include_directories (./
                     ../thirdPartyLibs/axisCore/utils/misc
                     ../thirdPartyLibs/axisCore/include
                     ../thirdPartyLibs/axisCore/src
                     ../thirdPartyLibs/axisCore/
                     ../thirdPartyLibs/axisCore/transport/bt)
set (SOURCES
        appMain.cpp
        CBTAdapter.cpp)

add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} ${LIBRARIES})


include(ExternalProject)
ExternalProject_Add(
   jsonCpp
   SOURCE_DIR  ../thirdPartyLibs/jsoncpp/
   DOWNLOAD_COMMAND ""
   CMAKE_COMMAND cmake
   BUILD_IN_SOURCE 1   
   BUILD_COMMAND make
   INSTALL_DIR ../thirdPartyLibs/jsoncpp/bin
   INSTALL_COMMAND make install
)

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
ADD_DEPENDENCIES(${PROJECT} ProtocolHandler)

install (TARGETS ${PROJECT} DESTINATION bin)
