include_directories (
  ../../../../src/components/utils/include
  ${GMOCK_INCLUDE_DIRECTORY}
  ../../../../src/components/smart_objects/include
  ../../../../src/components/formatters/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ../../../../src/components/formatters/include/
)

# Libraries are the same for all SchemaItem tests
set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    SmartObjects
    formatters
    jsoncpp
)

set (SOURCES
  ./test_BoolSchemaItemTest.cpp
  ./test_NumberSchemaItemTest.cpp
  ./test_StringSchemaItemTest.cpp
  ./test_ArraySchemaItemTest.cpp
  ./test_CObjectSchemaItem.cpp
  ./AlwaysTrueSchemaItemTest.cpp
  ./AlwaysFalseSchemaItemTest.cpp
)

add_library(test_SmartObjectSchema ${SOURCES})
create_test(test_SmartObjectSchemaTest "${SOURCES}" "${LIBRARIES}")
