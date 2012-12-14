include_directories (
  ./include
  ../../thirdPartyLibs/logger/include/
  ../../thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

set (SOURCES
    ./src/ClientSocket.cpp
    ./src/ServerSocket.cpp
    ./src/Socket.cpp
    ./src/SocketException.cpp
)

add_library("Utils" ${SOURCES})
