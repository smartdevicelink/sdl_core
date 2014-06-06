include_directories (
  ./include/
  ../transport_manager/include/
  ../protocol_handler/include/
  ../config_profile/include/
  ../security_manager/include/
  ../utils/include/
  ${ENCRYPTION_INCLUDE_DIRECTORY}
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

add_library(connectionHandler ${SOURCES})
target_link_libraries(connectionHandler encryption)
