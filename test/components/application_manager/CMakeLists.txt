include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/application_manager/include
  ../../../src/components/SmartObjects/include
  ../../../src/components/JSONHandler/include/
  ../../../src/components/utils/include/
  ../../../src//thirdPartyLibs/jsoncpp/include/
    ${CMAKE_BINARY_DIR}/src/components/
)

set(LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    ApplicationManager
    JSONHandler
    jsoncpp
    v4_protocol_v2_0_revT
    HMI_API
    SmartObjects
)

#create_test("test_APIVersionConverterV1Test" "./api_converter_v1_test.cpp" "${LIBRARIES}")

create_test("test_formatters_commands" "./formatters_commands.cc" "${LIBRARIES}")
# target_link_libraries ("test_show_command" log4cxx)
