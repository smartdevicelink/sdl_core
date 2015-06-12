include_directories (
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${CMAKE_SOURCE_DIR}/src/components/dbus/include
  ${CMAKE_SOURCE_DIR}/src/components/rpc_base/include
  ${GMOCK_INCLUDE_DIRECTORY}
  ${JSONCPP_INCLUDE_DIRECTORY}
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
  include_directories(${DBUS_INCLUDE_DIRS})
  set (LIBRARIES ${LIBRARIES} DBus)
  set (SOURCES ${SOURCES} rpc_base_dbus_test.cc)
endif ()

create_test("rpc_base_test" ${SOURCES}" "${LIBRARIES})
