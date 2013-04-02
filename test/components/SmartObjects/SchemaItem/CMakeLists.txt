include_directories (
  ../../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../../src/components/SmartObjects/include
  ../../../../src/thirdPartyLibs/logger/include/
  ../../../../src/thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

# Libraries are the same for all SchemaItem tests
set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    log4cplus
    SmartObjects
)

create_test("test_SmartObject_BoolSchemaItemTest"   "./test_BoolSchemaItemTest.cpp"   "${LIBRARIES}")
create_test("test_SmartObject_NumberSchemaItemTest" "./test_NumberSchemaItemTest.cpp" "${LIBRARIES}")
create_test("test_SmartObject_StringSchemaItemTest" "./test_StringSchemaItemTest.cpp" "${LIBRARIES}")
create_test("test_SmartObject_ArraySchemaItemTest"  "./test_ArraySchemaItemTest.cpp"  "${LIBRARIES}")
