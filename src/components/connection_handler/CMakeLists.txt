include_directories (
  ./include/
  ../transport_manager/include/
  ../protocol_handler/include/
  ../config_profile/include/
  ../utils/include/
  ../../thirdPartyLibs/encryption/include
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

set (SOURCES
    ./src/connection_handler_impl.cc
    ./src/connection.cc
    ./src/device.cc
    ./src/heartbeat_monitor.cc
)

set(LIBRARIES
    ConfigProfile
)

add_library("connectionHandler" ${SOURCES})
