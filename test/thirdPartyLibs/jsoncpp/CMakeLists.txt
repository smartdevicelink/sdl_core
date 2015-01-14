include_directories (
  ${GMOCK_INCLUDE_DIRECTORY}
  ${JSONCPP_INCLUDE_DIRECTORY}
  ./include
)

set (SOURCES
  ./src/json_reader_test.cc
)

add_library("test_JSONCPPTest" "${SOURCES}")
