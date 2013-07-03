include_directories (
  ${CMAKE_SOURCE_DIR}/src/components/utils/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/protocol_handler/include
  ../../../src/components/transport_manager/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/components/utils/include/
)

if(BUILD_ONLY_TM)
set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    TransportManager
    jsoncpp
    rt
)
set (SOURCES
  ../../../src/components/protocol_handler/src/raw_message.cc
  ./transport_manager_instance_test.cc
)

add_library("test_TransportManagerTest" ${SOURCES})
create_test("test_TransportManager_InstanceTest" "${SOURCES}" "${LIBRARIES}")
endif()
