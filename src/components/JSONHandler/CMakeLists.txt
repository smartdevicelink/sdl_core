cmake_minimum_required(VERSION 2.8)
project (jsonHandler) 

include_directories (./include/
                     ../../thirdPartyLibs/jsoncpp/include/
                     ../../thirdPartyLibs/axisCore/include
                     ../../thirdPartyLibs/axisCore/
                     ../../thirdPartyLibs/axisCore/src
                     ../../thirdPartyLibs/logger/include
                     ../../thirdPartyLibs/logger/log4cplus-1.1.0/include
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
                  ./src/Speak.cpp
                  ./src/SpeakMarshaller.cpp
                  ./src/TTSChunkMarshaller.cpp
                  ./src/SpeechCapabilitiesMarshaller.cpp
                  ./src/RPC2Request.cpp
                  ./src/RPC2Response.cpp
                  ./src/RPC2Error.cpp
                  ./src/SpeakResponseMarshaller.cpp
                  ./src/SpeakResponse.cpp
                  ./src/ResultMarshaller.cpp
                  ./src/Alert.cpp
                  ./src/AlertResponse.cpp
                  ./src/AlertMarshaller.cpp
                  ./src/AlertResponseMarshaller.cpp
                  ./src/Show.cpp
                  ./src/ShowResponse.cpp
                  ./src/ShowMarshaller.cpp
                  ./src/ShowResponseMarshaller.cpp
                  ./src/TextAlignmentMarshaller.cpp
                  ./src/GetCapabilities.cpp
                  ./src/GetCapabilitiesResponseMarshaller.cpp
                  ./src/GetCapabilitiesResponse.cpp
                  ./src/GetCapabilitiesMarshaller.cpp
                  ./src/GetCapabilities.cpp
                  ./src/ButtonNameMarshaller.cc
                  ./src/ButtonCapabilitiesMarshaller.cpp
                  ./src/OnButtonPress.cpp
                  ./src/OnButtonPressMarshaller.cpp
                  ./src/ButtonPressModeMarshaller.cc
                  ./src/SetGlobalPropertiesResponse.cpp
                  ./src/SetGlobalPropertiesResponseMarshaller.cpp
                  ./src/SetGlobalProperties.cpp
                  ./src/SetGlobalPropertiesMarshaller.cpp
           )

## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-O0 -g3 -ggdb3")        ## DON'T Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-m")  ## Strip binary
endif()

include(ExternalProject)
ExternalProject_Add(
   jsonCpp
   SOURCE_DIR  ../../thirdPartyLibs/jsoncpp/
   DOWNLOAD_COMMAND ""
   CMAKE_COMMAND cmake
   BUILD_IN_SOURCE 1   
   BUILD_COMMAND make
   INSTALL_DIR ../../thirdPartyLibs/jsoncpp/bin
   INSTALL_COMMAND make install
)

ExternalProject_Add(
   ProtocolHandler
   SOURCE_DIR ../../thirdPartyLibs/axisCore/
   DOWNLOAD_COMMAND ""
   CMAKE_COMMAND cmake
   BUILD_IN_SOURCE 1   
   BUILD_COMMAND make
   INSTALL_DIR ../../thirdPartyLibs/axisCore/bin
   INSTALL_COMMAND make install
)


set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_INSTALL_PREFIX .)
install (TARGETS jsonHandler DESTINATION bin)

