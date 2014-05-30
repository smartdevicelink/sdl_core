include_directories (
  ./include
  ../utils/include/
  ../config_profile/include/
  ../../thirdPartyLibs/MessageBroker/include/
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

set (SOURCES
    ./src/watchdog.cc
    ./src/request_watchdog.cc
    ./src/request_info.cc
)

add_library("RequestWatchdog" ${SOURCES})
target_link_libraries("RequestWatchdog" Utils)
