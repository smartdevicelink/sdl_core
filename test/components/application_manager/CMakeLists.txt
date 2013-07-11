############################## Subfolders #####################################
# --- Policies table test
add_subdirectory(./policies_manager)

###############################################################################

include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/application_manager/include
  ../../../src/components/SmartObjects/include
  ../../../src/components/formatters/include/
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
    formatters
    jsoncpp
    MOBILE_API
    HMI_API
    SmartObjects
)

#create_test("test_APIVersionConverterV1Test" "./api_converter_v1_test.cpp" "${LIBRARIES}")
create_test("test_formatters_commands" "./formatters_commands.cc" "${LIBRARIES}")
add_library("test_FormattersCommandsTest" "./formatters_commands.cc")
