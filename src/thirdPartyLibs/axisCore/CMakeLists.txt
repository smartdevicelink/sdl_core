include_directories (
  ./
  ./src
  ./include/
  ./utils/misc
  ../../components/TransportManager/include
  ../logger/include
  ../logger/log4cplus-1.1.0/include
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

set (SOURCES
  ./src/Message.cpp
  ./src/ProtocolHandler.cpp
  ./MessageGenerator/CMessage.cpp
)

add_library("ProtocolHandler" ${SOURCES})
