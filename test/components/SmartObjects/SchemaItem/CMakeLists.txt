include_directories (
  ../../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../../src/components/SmartObjects/include
  ../../../../src/thirdPartyLibs/logger/include/
  ../../../../src/thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

# Sources are separated for each SchemaItem test
set (SOURCES_BOOL_SCHEMA_ITEM_TEST
    ./test_BoolSchemaItemTest.cpp
)

set (SOURCES_NUMBER_SCHEMA_ITEM_TEST
    ./test_NumberSchemaItemTest.cpp
)

# Libraries are the same for all SchemaItem tests
set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    SmartObjects
    log4cplus
)

add_executable("test_BoolSchemaItemTest" ${SOURCES_BOOL_SCHEMA_ITEM_TEST})
target_link_libraries("test_BoolSchemaItemTest" ${LIBRARIES})
add_test(test_BoolSchemaItemTest test_BoolSchemaItemTest)

add_executable("test_NumberSchemaItemTest" ${SOURCES_NUMBER_SCHEMA_ITEM_TEST})
target_link_libraries("test_NumberSchemaItemTest" ${LIBRARIES})
add_test(test_NumberSchemaItemTest test_NumberSchemaItemTest)