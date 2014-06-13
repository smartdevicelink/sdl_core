include_directories(
  ${GMOCK_INCLUDE_DIRECTORY}
  ./include/
  ../../../src/thirdPartyLibs/gmock-1.7.0/include
  ../../../src/thirdPartyLibs/gmock-1.7.0/gtest/include
  ${SecurityManagerIncludeDir}
  ../../../src/components/transport_manager/include
  ../../../src/components/protocol_handler/include
  ../../../src/components/connection_handler/include
  ../../../src/components/utils/include
)

set(SOURCES
  ./src/crypto_manager_impl_test.cc
  ./src/security_manager_test.cc
  ./src/security_query_test.cc
  ./src/security_manager_mock.cc
)

set(RTLIB rt)
if(CMAKE_SYSTEM_NAME STREQUAL "QNX")
set(RTLIB )
endif()

set(LIBRARIES
  gtest
  gtest_main
  gmock
  gmock_main
  ${SecurityManagerLibrary}
  crypto
  ssl
  ProtocolHandler
  connectionHandler
  TransportManager
  Utils
  ${RTLIB}
)

add_library(test_SecurityManager ${SOURCES})
target_link_libraries(test_SecurityManager ${LIBRARIES} )
create_test(test_SecurityManagerTest "${SOURCES}" "${LIBRARIES}")

# vim: set ts=2 sw=2 et:
