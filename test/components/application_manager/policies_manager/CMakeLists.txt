include_directories (
  ../../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../../src/components/application_manager/include
  ../../../../src/components/smart_objects/include
  ../../../../src/components/utils/include/
  ../../../../src//thirdPartyLibs/jsoncpp/include/
  ../../../../test/components/application_manager/policies_manager/include
  ${CMAKE_BINARY_DIR}/src/components/
)

set (SOURCES
  ./src/policies_table_test.cc
)

set(LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    ApplicationManager
    jsoncpp
    Utils
    ConfigProfile
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/log4cxx.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/policy_table.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/smartDeviceLink.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
create_test("test_policies_table" "${SOURCES}" "${LIBRARIES}")
