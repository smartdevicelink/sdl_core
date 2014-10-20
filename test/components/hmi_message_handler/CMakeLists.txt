include_directories (
  ${GMOCK_INCLUDE_DIRECTORY}
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${CMAKE_SOURCE_DIR}/src/components/utils/include/
  ${CMAKE_SOURCE_DIR}/src/components/application_manager/include/
  ${CMAKE_SOURCE_DIR}/src/components/hmi_message_handler/include/
  ${CMAKE_SOURCE_DIR}/src/components/smart_objects/include/
  ${CMAKE_SOURCE_DIR}/test/components/hmi_message_handler/include/
  ${CMAKE_BINARY_DIR}/src/components/
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    Utils
    ProtocolHandler
    ApplicationManager
    SmartObjects
    jsoncpp
    HMIMessageHandler
    ${RTLIB}
    ProtocolLibrary
)

set (SOURCES
   #./src/mock_subscriber.cc
   #./src/test_dbus_message_adapter.cc
)

#create_test("test_DBusMessageAdapter" "${SOURCES}" "${LIBRARIES}")
create_test("test_mqueue_adapter" "./src/test_mqueue_adapter.cc" "${LIBRARIES}")

