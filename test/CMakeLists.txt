# --- GoogleTest example
#add_subdirectory(./gtest-example)

# --- components tests
add_subdirectory(./components)

include_directories (
  ../src/thirdPartyLibs/gmock-1.6.0/include
  ../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../src/components/request_watchdog/include
  ../src/components/protocol_handler/include
  ../src/components/TransportManager/include
  ../src/components/connection_handler/include
  ../src/components/JSONHandler/include
  ../src/components/SmartObjects/include
  ../src/thirdPartyLibs/jsoncpp/include/
  ../src/thirdPartyLibs/logger/include/
  ../src/components/utils/include/
  ../test/components/request_watchdog/include/
  ../test/components/protocol_handler/include/
  ../test/components/utils/include
  ../test/components/json_handler/include
  ${CMAKE_BINARY_DIR}
)

set(LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    RequestWatchdog
    JSONHandler
    jsoncpp
    test_JSONHandler_v4_protocol_v2_0_revP
    test_JSONHandlerTest
    test_UtilsTest
    test_RequestWatchdogTest
    test_ProtocolHandlerTest
    SmartObjects
    Utils
)

create_test("test_suit" "./test_suit.cc" "${LIBRARIES}")

