include_directories (
  ../../../src/components/policies/include
  ../../../src/components/smart_objects/include
  ../../../src/components/utils/include
  ../../../src/components/config_profile/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/thirdPartyLibs/jsoncpp/include/
  ../../../src/thirdPartyLibs/MessageBroker/src/
  ../../../test/components/
  ${CMAKE_BINARY_DIR}
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    SmartObjects
    formatters
    policies
    Utils
    ConfigProfile
    MessageBroker
)

set (SOURCES
  ./policies_draft_test.cc
)

add_library("test_policies_test" ${SOURCES})

create_test("test_policies_draft_test" "./policies_draft_test.cc" "${LIBRARIES}")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/log4cxx.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SDLPolicyTable_basic.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/bad.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


