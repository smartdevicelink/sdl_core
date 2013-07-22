include_directories (
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ${CMAKE_SOURCE_DIR}/src/components/transport_manager/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/logger/include/
  ${CMAKE_SOURCE_DIR}/src/components/utils/include/
  ${CMAKE_SOURCE_DIR}/src/components/protocol_handler/include/
  ${CMAKE_SOURCE_DIR}/test/components/transport_manager/include/
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
   ./src/transport_manager.cc
   ./src/mock_device_adapter.cc
)

#create_test("test_TransportManagerTest" "${SOURCES}" "${LIBRARIES}")
create_test("test_TcpDeviceAdapter" "src/test_tcp_device_adapter.cc" "${LIBRARIES}")
#target_link_libraries("test_TransportManagerTest" TransportManager)
endif()
