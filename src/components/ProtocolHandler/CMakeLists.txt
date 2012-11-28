include_directories (
  ./include
  ../Utils/include/
  ../TransportManager/include/
  ../../thirdPartyLibs/logger/include/
  ../../thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

set (SOURCES
    ./src/AppLinkRawMessage.cpp
    ./src/ProtocolHandler.cpp
    ./src/ProtocolPacket.cpp
)

add_library("ProtocolHandler" ${SOURCES})