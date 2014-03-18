include_directories (
  ../../../src/thirdPartyLibs/gmock-1.7.0/include
  ../../../src/thirdPartyLibs/gmock-1.7.0/gtest/include
  ../../../src/thirdPartyLibs/jsoncpp/include/
  ../../../test/thirdPartyLibs/jsoncpp/include/
)

set (SOURCES
  ./src/json_reader_test.cc
)

add_library("test_JSONCPPTest" "${SOURCES}")
