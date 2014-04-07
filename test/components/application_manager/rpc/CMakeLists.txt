find_package(Gstreamer-1.0 REQUIRED)
find_package(Glib-2.0 REQUIRED)
find_package(LibXML2 REQUIRED)

# required to find 'glibconfig.h'
find_package(PkgConfig)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
add_definitions(${GLIB2_CFLAGS})

include_directories (
  ./include
  ../../../../src/thirdPartyLibs/gmock-1.7.0/include
  ../../../../src/thirdPartyLibs/gmock-1.7.0/gtest/include
  ../../../../src/thirdPartyLibs/MessageBroker/include
  ../../../../src/thirdPartyLibs/jsoncpp/include
  ../../../../src/appMain
  ../../../../src/components/application_manager/include
  ../../../../src/components/protocol_handler/include
  ../../../../src/components/hmi_message_handler/include
  #../../../../src/components/mobile_message_handler/include/
  ../../../../src/components/security_manager/include
  ../../../../src/components/transport_manager/include
  ../../../../src/components/connection_handler/include
  ../../../../src/components/utils/include
  ../../../../src/components/smart_objects/include
  ../../../../src/components/request_watchdog/include
  ../../../../src/components/media_manager/include
  ../../../../src/components/formatters/include
  ../../../../src/components/config_profile/include
  ../../../../src/components/policies/include
  ${GSTREAMER_gst_INCLUDE_DIR}
  ${GLIB_glib_2_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/src/components/
  ${CMAKE_BINARY_DIR}/src/components/
  ${LibXML2_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/src/components/dbus/include
)

set(LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    ApplicationManager
    HMI_API
    MOBILE_API
    SmartObjects
    formatters
    ProtocolHandler
    connectionHandler
    TransportManager
    HMIMessageHandler
    MessageBroker
    MessageBrokerClient
    MessageBrokerServer
    Utils
    encryption
    jsoncpp
    bluetooth
    pthread
    rt
    ConfigProfile
    RequestWatchdog
    MediaManager
    ${LibXML2_LIBRARIES} -lxml2
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/XmlTests/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/XmlTests/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SyncProxyTester/ DESTINATION ${CMAKE_BINARY_DIR}/test/SyncProxyTester/)

add_library("test_App_Manager"
						"./src/admin_app_test.cc"
						"./src/gmock_class_container.cc"
						"./src/gtest_ford.cc"
						"./src/itest_argument.cc"
						"./src/init_hmi.cc"
						"./src/hmi_message_handler_tester.cc"
						"./src/xml_parser.cc"
						"./src/smoke_test_exp_media.cc"
						"./src/protocol_handler_mockup.cc"
						"./src/init_start_data.cc"
						"./src/test_hmi.cc"
                                                "./src/application_manager_impl_test.cc")
						
