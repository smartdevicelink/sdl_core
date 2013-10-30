include_directories (
  ../../../src/components/policies/include
  ../../../src/components/smart_objects/include
  ../../../src/components/utils/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/thirdPartyLibs/jsoncpp/include/
  ../../../test/components/
  ${CMAKE_BINARY_DIR}
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    SmartObjects
    policies
)

set (SOURCES
  ./policies_draft_test.cc
)

add_library("test_policies_test" ${SOURCES})

create_test("test_policies_draft_test" "./policies_draft_test.cc" "${LIBRARIES}")



