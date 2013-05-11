include_directories (
./include
  ../utils/include/
  ../../thirdPartyLibs/logger/include/
  ../../thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ../../thirdPartyLibs/MessageBroker/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

set (SOURCES
    ./src/request_watchdog.cc
    ./src/request_info.cc
)

add_library("RequestWatchdog" ${SOURCES})
target_link_libraries ("RequestWatchdog" log4cxx)
