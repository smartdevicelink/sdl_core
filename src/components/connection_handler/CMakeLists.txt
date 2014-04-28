include_directories (
  ./include/
  ../transport_manager/include/
  ../protocol_handler/include/
  ../config_profile/include/
  ../utils/include/
  ../../thirdPartyLibs/encryption/include
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
