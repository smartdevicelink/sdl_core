include_directories (
  ./include/
  ../TransportManager/include/
  ../protocol_handler/include/
  ../utils/include/
)

set (SOURCES
    ./src/connection_handler_impl.cc
    ./src/connection.cc
    ./src/device.cc
)

add_library("connectionHandler" ${SOURCES})
