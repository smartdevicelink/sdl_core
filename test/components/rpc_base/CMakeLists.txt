find_package(DBus)

include_directories (
  ${CMAKE_SOURCE_DIR}/src/components/dbus/include
  ${CMAKE_SOURCE_DIR}/src/components/rpc_base/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.7.0/gtest/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.7.0/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/jsoncpp/include
  ${DBUS_INCLUDE_DIRS}
)

set (LIBRARIES
    gmock
    gmock_main
    gtest
    gtest_main
    jsoncpp
)

set (SOURCES
  rpc_base_json_test.cc
  rpc_base_test.cc
)

if (${HMI_DBUS_API})
  # Build dbus tests
  find_package(DBus)
  include_directories(${DBUS_INCLUDE_DIRS})
  set (LIBRARIES ${LIBRARIES} DBus)
  set (SOURCES ${SOURCES} rpc_base_dbus_test.cc)
endif ()

create_test("test_rpc_base" "${SOURCES}" "${LIBRARIES}")
