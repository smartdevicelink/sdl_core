include_directories(
  ../../../test/components/protocol_handler/include
  ./protocol_handler/include
  ${GMOCK_INCLUDE_DIRECTORY}
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${SecurityManagerIncludeDir}
  ${SecurityManagerTestIncludeDir}
  ${CMAKE_SOURCE_DIR}/src/components/transport_manager/include
  ${CMAKE_SOURCE_DIR}/src/components/utils/include
  ${CMAKE_SOURCE_DIR}/src/components/protocol_handler/include
  ${CMAKE_SOURCE_DIR}/src/components/connection_handler/include
  ${CMAKE_SOURCE_DIR}/src/components/config_profile/include
  ${CMAKE_SOURCE_DIR}/test/components/protocol_handler/src
  ${CMAKE_SOURCE_DIR}/test/security_manager/include
  ${CMAKE_BINARY_DIR}/src/components/
)

set(LIBRARIES
  gtest
  gtest_main
  gmock
  gmock_main
  ${SecurityManagerLibrary}
  ProtocolHandler
  connectionHandler
  TransportManager
  Utils
  ConfigProfile
  ${RTLIB}
)

set(SOURCES
  src/protocol_handler_tm_test.cc
)

add_library(test_ProtocolHandlerTest ${SOURCES})
create_test(test_ProtocolHandler "${SOURCES}" "${LIBRARIES}")
