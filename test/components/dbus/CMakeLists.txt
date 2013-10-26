include_directories (
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ${CMAKE_SOURCE_DIR}/src/components/
  ${CMAKE_BINARY_DIR}/src/components/
  ${CMAKE_SOURCE_DIR}/src/components/dbus/include/
  ${CMAKE_SOURCE_DIR}/src/components/utils/include/
  ${CMAKE_SOURCE_DIR}/src/components/smart_objects/include/
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    DBus
    formatters
    SmartObjects
)

set (SCHEMA_SOURCES
   ./src/test_schema.cc
)

set (DBUS_ADAPTER_SOURCES
   ./src/test_dbus_adapter.cc
)

create_test("test_DBusSchema" "${SCHEMA_SOURCES}" "${LIBRARIES}")
create_test("test_DBusAdapter" "${DBUS_ADAPTER_SOURCES}" "${LIBRARIES}")
