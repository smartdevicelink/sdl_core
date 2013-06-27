include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/utils/include
  ../../../test/components/utils/include
)

set(LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    Utils
)

set (SOURCES
  ./src/file_system_tests.cc
)

create_test("test_Utils" "${SOURCES}" "${LIBRARIES}")

add_library("test_UtilsTest" "${SOURCES}")
