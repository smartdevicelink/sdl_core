include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/TransportManager/include
  ../../../src/components/Utils/include
  ../../../src/components/protocol_handler/include
  ../../../src/components/ConnectionHandler/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    ProtocolHandler
    connectionHandler
    TransportManager
    Utils
    bluetooth    
    log4cplus
)

create_test("interface_test" "./protocol_handler_tm_test.cpp" "${LIBRARIES}")