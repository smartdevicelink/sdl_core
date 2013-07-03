include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/transport_manager/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/components/utils/include/
)

if(BUILD_ONLY_TM)
set (SOURCES
  ./transport_manager_instance_test.cc
)

add_library("test_TransportManagerTest" ${SOURCES})
endif()
