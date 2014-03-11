include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/transport_manager/include
  ../../../src/components/utils/include
  ../../../src/components/protocol_handler/include
  ../../../src/components/connection_handler/include
  ../../../src/components/security_manager/include
  ../../../test/components/protocol_handler/include
  ${CMAKE_BINARY_DIR}/src/components/
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
)

set (SOURCES
  ./src/protocol_handler_tm_test.cc
)

create_test("test_ProtocolHandler" "${SOURCES}" "${LIBRARIES}")

add_library("test_ProtocolHandlerTest" "${SOURCES}")
