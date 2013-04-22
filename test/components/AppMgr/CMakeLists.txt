include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/AppMgr/include
  ../../../src/components/JSONHandler/include/
  ../../../src/components/ConnectionHandler/include/
  ../../../src/components/protocol_handler/include/
  ../../../src/components/TransportManager/include/
  ../../../src/components/SmartObjects/include
  ../../../src/components/Utils/include/
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ../../../src//thirdPartyLibs/jsoncpp/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

set(LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    SmartObjects
    AppMgr
    log4cplus
    JSONHandler
    jsoncpp
)

create_test("test_AppMgr_ApplicationCoreTest" "./CApplicationCoreTest.cpp" "${LIBRARIES}")
