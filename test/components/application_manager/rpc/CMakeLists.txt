find_package(Gstreamer-1.0 REQUIRED)
find_package(Glib-2.0 REQUIRED)
find_package(LibXML2 REQUIRED)

# required to find 'glibconfig.h'
find_package(PkgConfig)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
add_definitions(${GLIB2_CFLAGS})

include_directories (
  ./include
  ${GMOCK_INCLUDE_DIRECTORY}
  ${MESSAGE_BROKER_INCLUDE_DIRECTORY}
  ${JSONCPP_INCLUDE_DIRECTORY}
  ../../../../src/appMain
  ../../../../src/components/application_manager/include
  ../../../../src/components/hmi_message_handler/include
  ../../../../src/components/smart_objects/include
  ../../../../src/components/request_watchdog/include
  ../../../../src/components/media_manager/include
  ../../../../src/components/formatters/include
  ../../../../src/components/config_profile/include
  ../../../../src/components/policy/src/policy/include
  ../../../../src/components/policy/src/policy/usage_statistics/include
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
    HMIMessageHandler
    MessageBroker
    MessageBrokerClient
    MessageBrokerServer
    Utils
    encryption
    jsoncpp
    bluetooth
    pthread
    ConfigProfile
    RequestWatchdog
    MediaManager
    ${LibXML2_LIBRARIES} -lxml2
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/XmlTests/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/XmlTests/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SyncProxyTester/ DESTINATION ${CMAKE_BINARY_DIR}/test/SyncProxyTester/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SyncProxyTester/icon.png DESTINATION ${CMAKE_BINARY_DIR}/test/AppManagerImplTest/)

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
            "./src/application_manager_impl_test.cc"
)

