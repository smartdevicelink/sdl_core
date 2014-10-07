include_directories (
  ${GMOCK_INCLUDE_DIRECTORY}
  ../../../src/components/utils/include
  ../../../test/components/utils/include
)

set(LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    Utils
    ConfigProfile
)

set(SOURCES
  ./src/prioritized_queue_tests.cc
)

create_test("test_Utils" "${SOURCES}" "${LIBRARIES}")

add_library("test_UtilsTest" ${SOURCES})
