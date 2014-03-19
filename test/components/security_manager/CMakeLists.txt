include_directories (
  ./include/
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/security_manager/include
  ../../../src/components/transport_manager/include
  ../../../src/components/protocol_handler/include
  ../../../src/components/connection_handler/include
  ../../../src/components/security_manager/include
  ../../../src/components/utils/include
)

set (SOURCES
  ./src/crypto_manager_impl_test.cc
  ./src/security_manager_test.cc
  ./src/security_query_test.cc
  ./security_manager_mock.cc
)

set (LIBRARIES
  gtest
  gtest_main
  gmock
  gmock_main
  SecurityManager
  crypto
  ssl
  ProtocolHandler
  connectionHandler
  TransportManager
  Utils
)

add_library(test_SecurityManager ${SOURCES})
target_link_libraries(test_SecurityManager ${LIBRARIES} )
create_test(test_SecurityManagerTest "${SOURCES}" "${LIBRARIES}")

# vim: set ts=2 sw=2 et:
