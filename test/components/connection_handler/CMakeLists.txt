include_directories (
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ./include
  ${CMAKE_SOURCE_DIR}/src/components/connection_handler/include
  ${CMAKE_SOURCE_DIR}/src/components/protocol_handler/include
  ${CMAKE_SOURCE_DIR}/src/components/utils/include
  ../../../src/components/utils/include
  ../../../src/components/config_profile/include
  ../security_manager/include
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    ProtocolHandler
    connectionHandler
    Utils
    ConfigProfile
    ProtocolLibrary
)

set (SOURCES
  ./src/connection_handler_impl_test.cc
  ./src/connection_test.cc
  ./src/heart_beat_monitor_test.cc
)

create_test("test_ConnectionHandler" "${SOURCES}" "${LIBRARIES}")

add_library("test_ConnectionHandlerTest" ${SOURCES})
