include_directories (
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ${CMAKE_SOURCE_DIR}/src/components/transport_manager/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/logger/include/
  ${CMAKE_SOURCE_DIR}/src/components/utils/include/
  ${CMAKE_SOURCE_DIR}/src/components/protocol_handler/include/
)

if(BUILD_ONLY_TM)
set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    TransportManager
    ProtocolHandler
    jsoncpp
    rt
)
set (SOURCES
  ./transport_manager_instance_test.cc
)

add_library("test_TransportManagerTest" ${SOURCES})
create_test("test_TransportManager_InstanceTest" "${SOURCES}" "${LIBRARIES}")
endif()
