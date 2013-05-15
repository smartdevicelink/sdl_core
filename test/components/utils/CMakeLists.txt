include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/utils/include
)

set(LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    Utils
)

create_test("FileSystemTest" "./file_system_tests.cc" "${LIBRARIES}")
