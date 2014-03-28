include(${CMAKE_SOURCE_DIR}/tools/intergen/GenerateInterfaceLibrary.cmake)

if (${HMI_DBUS_API})
  GenerateInterfaceLibrary("test_interface.xml" test_rpc_interface DBUS_SUPPORT)
else()
  GenerateInterfaceLibrary("test_interface.xml" test_rpc_interface)
endif()

set (TEST_HMI_INTERFACES
  common
  buttons
  navigation
  basic_communication
  tts
  ui
  vehicle_info
  vr
)

if (${HMI_DBUS_API})
  GenerateInterfaceLibrary("test_hmi_interafce.xml" "${TEST_HMI_INTERFACES}" AUTO_FUNC_IDS DBUS_SUPPORT)
else()
  GenerateInterfaceLibrary("test_hmi_interafce.xml" "${TEST_HMI_INTERFACES}" AUTO_FUNC_IDS)
endif()

include_directories (
  ${CMAKE_SOURCE_DIR}/src/components/rpc_base/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.7.0/gtest/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.7.0/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/jsoncpp/include
  ${CMAKE_CURRENT_BINARY_DIR}
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    test_rpc_interface
)

set (SOURCES
  src/generated_interface_json_tests.cc
)

if (${HMI_DBUS_API})
  # Build dbus tests
  find_package(DBus)
  include_directories(
      ${CMAKE_SOURCE_DIR}/src/components/dbus/include
      ${DBUS_INCLUDE_DIRS}
  )
  set (LIBRARIES ${LIBRARIES} DBus)
  set (SOURCES ${SOURCES} src/generated_interface_dbus_tests.cc)
endif ()


create_test(test_generated_interface "${SOURCES}" "${LIBRARIES}")
