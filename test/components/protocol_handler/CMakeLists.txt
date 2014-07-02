include_directories (
  ${GMOCK_INCLUDE_DIRECTORY}
  ../../../src/components/transport_manager/include
  ../../../src/components/utils/include
  ../../../src/components/protocol_handler/include
  ../../../src/components/connection_handler/include
  ../../../test/components/protocol_handler/include
  ${LOG4CXX_INCLUDE_DIRECTORY}
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
