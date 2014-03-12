find_package(Gstreamer-1.0 REQUIRED)
find_package(Glib-2.0 REQUIRED)
#find_package(LibXML2 REQUIRED)

# required to find 'glibconfig.h'
find_package(PkgConfig)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
add_definitions(${GLIB2_CFLAGS})

include_directories (
  ./include
  ../../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../../src/thirdPartyLibs/MessageBroker/include
  ../../../../src/thirdPartyLibs/jsoncpp/include
  ../../../../src/components/application_manager/include
  ../../../../src/components/protocol_handler/include
  ../../../../src/components/hmi_message_handler/include
  #../../../../src/components/mobile_message_handler/include/
  ../../../../src/components/transport_manager/include
  ../../../../src/components/connection_handler/include
  ../../../../src/components/utils/include
  ../../../../src/components/smart_objects/include
  ../../../../src/components/request_watchdog/include
  ../../../../src/components/media_manager/include
  ../../../../src/components/formatters/include
  ../../../../src/components/config_profile/include
  ../../../../src/components/policies/include
  #../../../../test/components/application_manager/rpc/include
  ${CMAKE_SOURCE_DIR}/src/components/
  ${CMAKE_BINARY_DIR}/src/components/
  #${LibXML2_INCLUDE_DIR}
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
    avahi-client
    avahi-common
    ConfigProfile
    #MobileMessageHandler
    RequestWatchdog
    MediaManager
    Libusb-1.0.16
    #${LibXML2_LIBRARIES} -lxml2
)


#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/log4cxx.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/XmlTests/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/XmlTests/)
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/hmi_link DESTINATION ${CMAKE_CURRENT_BINARY_DIR})



#add_library("test_App_Manager" "./src/app_manager_test.cc")

#add_library("test_Admin_App_Test" "./src/AdminAppTest.cc")
#add_library("test_G_Mock_Class_Container" "./src/GMockClassContainer.cc")
#add_library("test_G_Test_Ford" "./src/GTestFord.cc")
#add_library("test_Init_HMI" "./src/InitHMI.cc")
#add_library("test_HMI_Message_Handler_Tester" "./src/HMIMessageHandlerTester.cc")
#add_library("test_Mobile_Message_Handler_Tester" "./src/MobileMessageHandlerTester.cc")
#add_library("test_Check_Mobile_Message_Handler" "./src/CkeckMobileMessageHandler.cc")
#add_library("test_XmlParser" "./src/xmlParser.cc")

add_library("test_App_Manager"
						"./src/AdminAppTest.cc"
						"./src/GMockClassContainer.cc"
						"./src/GTestFord.cc"
						"./src/ICheckClass.cc"
						"./src/InitHMI.cc"
						"./src/HMIMessageHandlerTester.cc"
						#"./src/MobileMessageHandlerTester.cc"
						#"./src/CkeckMobileMessageHandler.cc"
						#"./src/testMobile.cc"
						#"./src/xmlParser.cc"
						#"./src/too_many_request_test_case.cc"
						#"./src/too_many_pending_requests_test_case.cc"
						"./src/addTest.cc"
						"./src/initStartData.cc"
						"./src/testHMI.cc")
