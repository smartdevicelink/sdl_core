find_package(Gstreamer-1.0 REQUIRED)
find_package(Glib-2.0 REQUIRED)
find_package(LibXML2 REQUIRED)

# --- GoogleTest example
#add_subdirectory(./gtest-example)

# --- components tests
add_subdirectory(./components)
add_subdirectory(./thirdPartyLibs)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/components/policy/test/policy ./policy)
add_subdirectory(./tools)


include_directories (
  ../src/thirdPartyLibs/gmock-1.7.0/include
  ../src/thirdPartyLibs/jsoncpp/include/
  ../src/thirdPartyLibs/gmock-1.7.0/gtest/include
  ../src/thirdPartyLibs/MessageBroker/include
  ../src/appMain
  ../src/components/application_manager/include
  ../src/components/hmi_message_handler/include
  ../src/components/request_watchdog/include
  ../src/components/media_manager/include
  ../src/components/config_profile/include
  ../src/components/protocol_handler/include
  ../src/components/transport_manager/include
  ../src/components/connection_handler/include
  ../src/components/formatters/include
  ../src/components/smart_objects/include
  ../src/components/formatters/include
  ../src/components/utils/include/
  ../src/components/resumption/include/
  ../test/components/mobile_message_handler/include/
  ../test/components/request_watchdog/include/
  ../test/components/media_manager/include
  ../test/components/protocol_handler/include/
  ../test/components/utils/include
  ../test/components/json_handler/include
  ../test/components/smart_objects
  ../test/components/transport_manager
  ../test/components/
  ../test/thirdPartyLibs/jsoncpp/include
  ../test/components/application_manager
  ../test/components/application_manager/rpc/include
  ${CMAKE_BINARY_DIR}/src/components/
  ${CMAKE_BINARY_DIR}
  ${LibXML2_INCLUDE_DIR}
#  /usr/lib/i386-linux-gnu/glib-2.0/include
#  /usr/lib/x86_64-linux-gnu/glib-2.0/include/
#  ${GSTREAMER_gst_INCLUDE_DIR}
#  ${GLIB_glib_2_INCLUDE_DIR}
)

if (BUILD_TESTS_WITH_HMI)
 set(LIBRARIES
  test_App_Manager
  ApplicationManager
    MediaManager
    gtest
    gtest_main
    gmock
    gmock_main
    formatters
    HMI_API
    v4_protocol_v1_2_no_extra
    SmartObjects
    RequestWatchdog
    policies
    ProtocolHandler
    Utils
    ConfigProfile
    #test_JSONHandler_v4_protocol_v2_0_revP
    #test_json_handler
    #test_SmartObjectTest
    #test_FormattersCommandsTest
    #test_UtilsTest
    #test_RequestWatchdogTest
    #test_ProtocolHandlerTest
    #test_JSONCPPTest
    #test_MobileMessageHandlerTest
    connectionHandler
    MOBILE_API
    jsoncpp
#################################################
    TransportManager
    HMIMessageHandler
    MessageBroker
    MessageBrokerClient
    MessageBrokerServer
    encryption
    bluetooth
    pthread
    avahi-client
    avahi-common
    ${LibXML2_LIBRARIES} -lxml2
    # rt
    # ${GSTREAMER_gstreamer_LIBRARY}
    Resumption
   )
else(BUILD_TESTS_WITH_HMI)
set(LIBRARIES
    MediaManager
    ApplicationManager
    gtest
    gtest_main
    gmock
    gmock_main
    formatters
    HMI_API
    v4_protocol_v1_2_no_extra
    SmartObjects
    RequestWatchdog
    #policy
    ProtocolHandler
    Utils
    ConfigProfile
    #test_JSONHandler_v4_protocol_v2_0_revP
    #test_json_handler
    #test_SmartObjectTest
    #test_FormattersCommandsTest
    #test_UtilsTest
    #test_RequestWatchdogTest
    #test_ProtocolHandlerTest
    #test_JSONCPPTest
    #test_MobileMessageHandlerTest
    connectionHandler
    MOBILE_API
    jsoncpp
#################################################
    TransportManager
    HMIMessageHandler
    MessageBroker
    MessageBrokerClient
    MessageBrokerServer
    encryption
    bluetooth
    pthread
    avahi-client
    avahi-common
    ${LibXML2_LIBRARIES} -lxml2
    # rt
    # ${GSTREAMER_gstreamer_LIBRARY}
    Resumption
)
endif(BUILD_TESTS_WITH_HMI)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  list(APPEND LIBRARIES dl)
endif()


if (BUILD_USB_SUPPORT)
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  list(APPEND LIBRARIES Libusb-1.0.16)
endif()
endif()

add_executable("test_suit" "./test_suit.cc")
target_link_libraries("test_suit" ${LIBRARIES})


# vim: set ts=2 sw=2 et:
