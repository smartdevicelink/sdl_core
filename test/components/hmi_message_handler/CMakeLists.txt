include_directories (
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/jsoncpp/include/
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/logger/include/
  ${CMAKE_SOURCE_DIR}/src/components/utils/include/
  ${CMAKE_SOURCE_DIR}/src/components/hmi_message_handler/include/
  ${CMAKE_SOURCE_DIR}/test/components/hmi_message_handler/include/
  /usr/include/dbus-1.0/
  /usr/lib/i386-linux-gnu/dbus-1.0/include/
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    Utils
    jsoncpp
    dbus-1
    HMIMessageHandler
)

set (SOURCES
   ./src/mock_subscriber.cc
   ./src/test_dbus_message_controller.cc
#   ./src/test_dbus_message_adapter.cc
)

create_test("test_DBusMessageController" "${SOURCES}" "${LIBRARIES}")
#create_test("test_DBusMessageAdapter" "${SOURCES}" "${LIBRARIES}")

#target_link_libraries("test_DBusMessageAdapter" DBusMessageAdapter)
target_link_libraries("test_DBusMessageController" ${LIBRARIES})
