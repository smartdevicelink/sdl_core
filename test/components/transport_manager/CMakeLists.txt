include_directories (
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.7.0/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.7.0/gtest/include
  ${CMAKE_SOURCE_DIR}/src/components/transport_manager/include
  ${CMAKE_SOURCE_DIR}/src/components/utils/include/
  ${CMAKE_SOURCE_DIR}/src/components/config_profile/include/
  ${CMAKE_SOURCE_DIR}/src/components/protocol_handler/include/
  ${CMAKE_SOURCE_DIR}/src/components/security_manager/include
  ${CMAKE_SOURCE_DIR}/src/components/connection_handler/include/
  ${CMAKE_SOURCE_DIR}/test/components/transport_manager/include/
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/libusbx-1.0.16/libusb/
)

set(RTLIB rt)
if(CMAKE_SYSTEM_NAME STREQUAL "QNX")
set(RTLIB )
endif()

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    TransportManager
    ProtocolHandler
    Resumption
    Utils
    ConfigProfile
    jsoncpp
    
    ${RTLIB}
)

if (BUILD_AVAHI_SUPPORT)
  list(APPEND LIBRARIES avahi-client)
endif()

if (BUILD_USB_SUPPORT)
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  list(APPEND LIBRARIES Libusb-1.0.16)
endif()
endif()

set (SOURCES
   ./src/mock_connection.cc
   ./src/mock_connection_factory.cc
   ./src/mock_application.cc
   ./src/mock_device.cc
   ./src/mock_transport_adapter.cc
   ./src/mock_device_scanner.cc
   ./src/raw_message_matcher.cc
   ./src/transport_manager.cc
)

set (TESTUSBSOURCES
   ./src/test_usb.cc
)

create_test("test_TransportManagerTest" "${SOURCES}" "${LIBRARIES}")
create_test("test_TcpTransportAdapter" "src/test_tcp_transport_adapter.cc" "${LIBRARIES}")
#create_test("test_usb" "${TESTUSBSOURCES}" "${LIBRARIES}")

#add_executable("test_DnssdServiceDiscovery" "src/test_dnssd_service_browser.cc")
#target_link_libraries("test_DnssdServiceDiscovery" ${LIBRARIES})
#add_test(test_DnssdServiceDiscovery, ./test_DnssdServiceDiscovery.sh)

target_link_libraries("test_TransportManagerTest" TransportManager)

if (BUILD_USB_SUPPORT)
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_link_libraries("test_TransportManagerTest" Libusb-1.0.16)
endif()
endif()
# vim: set ts=2 sw=2 et:
