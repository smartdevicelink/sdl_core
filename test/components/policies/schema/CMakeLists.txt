include_directories (
  ${CMAKE_SOURCE_DIR}/src/components/policies/include
  ${CMAKE_SOURCE_DIR}/src/components/smart_objects/include
  ${CMAKE_SOURCE_DIR}/src/components/formatters/include/
  ${CMAKE_SOURCE_DIR}/src/components/utils/include
  ${CMAKE_SOURCE_DIR}/src/components/config_profile/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/logger/include/
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/jsoncpp/include/
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/MessageBroker/src/
  ${CMAKE_SOURCE_DIR}/test/components/
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

create_test("test_policy_table_schema" "./policy_table_schema_test.cc" "${LIBRARIES}")
create_test("test_preloaded_pt_schema" "./preloaded_pt_schema_test.cc" "${LIBRARIES}")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/valid_policy_table.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/valid_preloaded_pt_1.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/valid_preloaded_pt_2.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
