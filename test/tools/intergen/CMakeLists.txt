message("Intergen tests cmake")
include(${CMAKE_SOURCE_DIR}/tools/intergen/GenerateInterfaceLibrary.cmake)
GenerateInterfaceLibrary("test_interface.xml" test_rpc_interface)

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
GenerateInterfaceLibrary("test_hmi_interafce.xml" "${TEST_HMI_INTERFACES}" AUTO_FUNC_IDS)

include_directories (
  ${CMAKE_SOURCE_DIR}/src/components/rpc_base/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.7.0/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.7.0/gtest/include
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
  src/generated_interface_tests.cc
)

create_test(test_generated_interface "${SOURCES}" "${LIBRARIES}")
