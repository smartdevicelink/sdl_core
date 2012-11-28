include_directories (
  ./include/
  ../TransportManager/include/
  ../TransportManager/include/TransportManager
  ../ProtocolHandler/include/ProtocolHandler
  ../connectionHandler/include/connectionHandler
  ../../thirdPartyLibs/logger/include
  ../../thirdPartyLibs/logger/log4cplus-1.1.0/include
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

set (SOURCES
    ./src/CConnectionHandler.cpp
    ./src/CConnectionHandlerObserver.cpp
)

add_library("connectionHandler" ${SOURCES})
