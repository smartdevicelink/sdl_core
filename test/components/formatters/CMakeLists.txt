include_directories (
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ../../../src/components/smart_objects/include
  ../../../src/components/formatters/include
  ../../../src/components/utils/include
  ${JSONCPP_INCLUDE_DIRECTORY}
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
