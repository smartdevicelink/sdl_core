include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/TransportManager/include
  ../../../src/components/utils/include
  ../../../src/components/protocol_handler/include
  ../../../src/components/connection_handler/include
  ../../../test/components/protocol_handler/include
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
)

set (SOURCES
  ./src/protocol_handler_tm_test.cc
)

add_library("test_ProtocolHandlerTest" "${SOURCES}")
