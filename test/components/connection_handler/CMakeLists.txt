include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/transport_manager/include
  ../../../src/components/utils/include
  ../../../src/components/protocol_handler/include
  ../../../src/components/connection_handler/include
  ../../../src/components/security_manager/include
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
    bluetooth
    ConfigProfile
)

set (SOURCES
  ./src/connection_handler_impl_test.cc
  ./src/connection_test.cc
  ./src/heart_beat_monitor_test.cc
)

create_test("test_ConnectionHandler" "${SOURCES}" "${LIBRARIES}")

add_library("test_ConnectionHandlerTest" ${SOURCES})
