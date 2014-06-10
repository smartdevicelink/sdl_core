include_directories (
  ${GMOCK_INCLUDE_DIRECTORY}
  ../../../src/components/transport_manager/include
  ../../../src/components/utils/include
  ../../../src/components/protocol_handler/include
  ../../../src/components/connection_handler/include
  ../../../test/components/connection_handler/include
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
  ./src/heart_beat_monitor_test.cc
)

create_test("test_ConnectionHandler" "${SOURCES}" "${LIBRARIES}")

add_library("test_ConnectionHandlerTest" "${SOURCES}")
