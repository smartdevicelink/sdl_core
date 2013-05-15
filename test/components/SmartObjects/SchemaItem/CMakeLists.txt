include_directories (
  ../../../../src/components/utils/include
  ../../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../../src/components/SmartObjects/include
  ../../../../src/components/JSONHandler/include/
  ../../../../src//thirdPartyLibs/jsoncpp/include/
)

# Libraries are the same for all SchemaItem tests
set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    jsoncpp
    SmartObjects
    JSONHandler
)

create_test("test_SmartObject_BoolSchemaItemTest"           "./test_BoolSchemaItemTest.cpp"   "${LIBRARIES}")
create_test("test_SmartObject_NumberSchemaItemTest"         "./test_NumberSchemaItemTest.cpp" "${LIBRARIES}")
create_test("test_SmartObject_StringSchemaItemTest"         "./test_StringSchemaItemTest.cpp" "${LIBRARIES}")
create_test("test_SmartObject_ArraySchemaItemTest"          "./test_ArraySchemaItemTest.cpp"  "${LIBRARIES}")
create_test("test_SmartObject_EnumSchemaItemTest"           "./EnumSchemaItemTest.cpp"  "${LIBRARIES}")
create_test("test_SmartObject_ObjectSchemaItemTest"         "./ObjectSchemaItemTest.cpp"  "${LIBRARIES}")
create_test("test_SmartObject_AlwaysTrueSchemaItemTest"     "./AlwaysTrueSchemaItemTest.cpp"  "${LIBRARIES}")
create_test("test_SmartObject_AlwaysFalseSchemaItemTest"    "./AlwaysFalseSchemaItemTest.cpp"  "${LIBRARIES}")
