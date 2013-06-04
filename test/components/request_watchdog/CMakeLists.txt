include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/request_watchdog/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/components/utils/include/
  ../../../test/components/request_watchdog/include
)

set (SOURCES
  ./src/request_watchdog_test.cc
)

add_library("test_RequestWatchdogTest" "${SOURCES}")
