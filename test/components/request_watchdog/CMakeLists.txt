include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/request_watchdog/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/components/utils/include/
)

set(LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    RequestWatchdog
    Utils
)

create_test("test_RequestWatchdogTest" "./request_watchdog_test.cpp" "${LIBRARIES}")
