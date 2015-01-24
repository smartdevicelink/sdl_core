############################## Subfolders #####################################
# --- Policies table test
#add_subdirectory(./policies_manager)

###############################################################################

include_directories (
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ${CMAKE_SOURCE_DIR}/src/components/utils/include
  ../../../src/components/application_manager/include
  ../../../src/components/smart_objects/include
  ../../../src/components/formatters/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
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
    ProtocolLibrary
)

#create_test("test_APIVersionConverterV1Test" "./api_converter_v1_test.cpp" "${LIBRARIES}")
create_test("test_formatters_commands" "./formatters_commands.cc" "${LIBRARIES}")
#create_test("test_schema_factory_test" "./schema_factory_test.cc" "${LIBRARIES}")
add_library("test_FormattersCommandsTest" "./formatters_commands.cc")
