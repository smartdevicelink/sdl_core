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

set (SOURCES_STRING_SCHEMA_ITEM_TEST
    ./test_StringSchemaItemTest.cpp
)

set (SOURCES_ARRAY_SCHEMA_ITEM_TEST
    ./test_ArraySchemaItemTest.cpp
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

create_test("test_SmartObject_BoolSchemaItemTest" ${SOURCES_BOOL_SCHEMA_ITEM_TEST} ${LIBRARIES})
create_test("test_SmartObject_NumberSchemaItemTest" ${SOURCES_NUMBER_SCHEMA_ITEM_TEST} ${LIBRARIES})
create_test("test_SmartObject_StringSchemaItemTest" ${SOURCES_STRING_SCHEMA_ITEM_TEST} ${LIBRARIES})
create_test("test_SmartObject_ArraySchemaItemTest" ${SOURCES_ARRAY_SCHEMA_ITEM_TEST} ${LIBRARIES})
