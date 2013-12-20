include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/smart_objects/include
  ../../../src/components/formatters/include
  ../../../src/components/utils/include
  ../../../src//thirdPartyLibs/jsoncpp/include/
  ${CMAKE_BINARY_DIR}
)

set (LIBRARIES
    gtest
    gtest_main
    SmartObjects
    formatters
    jsoncpp
)

create_test("test_generic_json_formatter" "./src/generic_json_formatter_test.cc" "${LIBRARIES}")
