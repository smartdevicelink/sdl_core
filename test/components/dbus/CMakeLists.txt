include_directories (
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ${CMAKE_SOURCE_DIR}/src/components/
  ${CMAKE_SOURCE_DIR}/src/components/dbus/include/
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    DBus
)

set (SOURCES
   ./src/test_schema.cc
#   ./src/test_dbus_message_adapter.cc
)

create_test("test_DBusSchema" "${SOURCES}" "${LIBRARIES}")
#create_test("test_DBusMessageAdapter" "${SOURCES}" "${LIBRARIES}")

#target_link_libraries("test_DBusMessageAdapter" DBusMessageAdapter)
#target_link_libraries("test_DBusMessageController" ${LIBRARIES})
