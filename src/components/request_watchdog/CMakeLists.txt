include_directories (
  ./include
  ../utils/include/
  ../config_profile/include/
  ${MESSAGE_BROKER_INCLUDE_DIRECTORY}
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

set (SOURCES
    ./src/watchdog.cc
    ./src/request_watchdog.cc
    ./src/request_info.cc
)

add_library("RequestWatchdog" ${SOURCES})
target_link_libraries("RequestWatchdog" Utils)
