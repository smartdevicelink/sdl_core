# Generating interface
GenerateInterface("test_v4_protocol_v2_0_revP.xml" "Gen::test::components::JSONHandler2" "alrpcv2")
GenerateInterface("test_v4_protocol_v1_2_no_extra.xml" "Gen::test::components::JSONHandler1" "alrpcv1")

# --- Formatters
add_subdirectory(./formatters)

include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/SmartObjects/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ../../../src//thirdPartyLibs/jsoncpp/include/
  ../../../src/components/JSONHandler/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
  ${CMAKE_BINARY_DIR}
)

set (SOURCES_DRAFT_TEST
    ./SmartSchemaDraftTest.cpp
)

set(LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    test_v4_protocol_v2_0_revP
    SmartObjects
    log4cplus
    JSONHandler
    jsoncpp
  #v4_protocol_v2_0_revP
)

add_executable("test_SmartSchemaDraftTest" ${SOURCES_DRAFT_TEST})
target_link_libraries("test_SmartSchemaDraftTest" ${LIBRARIES})
add_test(test_SmartSchemaDraftTest test_SmartSchemaDraftTest)