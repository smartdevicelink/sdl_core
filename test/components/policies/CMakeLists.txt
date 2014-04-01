# policy table schema tests
add_subdirectory(schema)

include_directories (
  ${CMAKE_SOURCE_DIR}/src/components/policies/include
  ${CMAKE_SOURCE_DIR}/src/components/smart_objects/include
  ${CMAKE_SOURCE_DIR}/src/components/utils/include
  ${CMAKE_SOURCE_DIR}/src/components/config_profile/include
  ${CMAKE_SOURCE_DIR}/src/components/formatters/include/
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.7.0/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.7.0/gtest/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/logger/include/
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/jsoncpp/include/
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/MessageBroker/src/
  ${CMAKE_SOURCE_DIR}/test/components/
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/src/components/
)

set(RTLIB rt)
if(CMAKE_SYSTEM_NAME STREQUAL "QNX")
set(RTLIB )
endif()
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
    MOBILE_API
    ${RTLIB}
)

create_test("test_policies_draft_test" "./policies_draft_test.cc" "${LIBRARIES}")
create_test("test_generic_policy_manager_test" "./generic_policy_manager_test.cc" "${LIBRARIES}")
create_test("test_wp1_integration_test" "./wp1_integration_test.cc" "${LIBRARIES}")
create_test("test_policies_performance" "./performance_test.cc" "${LIBRARIES}")


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/log4cxx.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SDLPolicyTable_basic.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/bad.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/src/appMain/wp1_policy_table.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
