include_directories(
  ${GMOCK_INCLUDE_DIRECTORY}
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ./include
  ${CMAKE_SOURCE_DIR}/src/components/protocol_handler/include
  ${SecurityManagerIncludeDir}
  ${CMAKE_SOURCE_DIR}/src/components/config_profile/include
)

set(LIBRARIES
  gtest
  gtest_main
  gmock
  gmock_main
  ProtocolHandler
  connectionHandler
  Utils
  ConfigProfile
  ${RTLIB}
  ProtocolLibrary
)

set(SOURCES
  src/protocol_handler_tm_test.cc
)

create_test(test_ProtocolHandler "${SOURCES}" "${LIBRARIES}")
add_library(test_ProtocolHandlerTest ${SOURCES})
