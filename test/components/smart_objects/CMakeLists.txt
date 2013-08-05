# --- SchemaItems
add_subdirectory(./SchemaItem)

include_directories (
  ${CMAKE_SOURCE_DIR}/src/components/utils/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/smart_objects/include
  ../../../src/components/formatters/include/
  ../../../src//thirdPartyLibs/jsoncpp/include/
)

# Libraries are the same for all SmartObject test
set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    SmartObjects
    formatters
    jsoncpp
    rt
)

set (SOURCES
  ./SmartObjectConvertionTimeTest.cc
  ./SmartObjectDraftTest.cc
  ./SmartObjectInvalidTest.cc
  ./SmartObjectStressTest.cc
  ./SmartObjectUnitTest.cc
  ./TSharedPtrTest.cc
)

add_library("test_SmartObjectTest" ${SOURCES})

create_test("test_SmartObject_DraftTest" "./SmartObjectDraftTest.cc" "${LIBRARIES}")
create_test("test_SmartObject_UnitTest" "./SmartObjectUnitTest.cc" "${LIBRARIES}")
create_test("test_SmartObject_StressTest" "./SmartObjectStressTest.cc" "${LIBRARIES}")
create_test("test_SmartObject_InvalidTest" "./SmartObjectInvalidTest.cc" "${LIBRARIES}")
create_test("test_SmartObject_ConvertionTimeTest" "./SmartObjectConvertionTimeTest.cc" "${LIBRARIES}")
create_test("test_TSharedPtrTest" "./TSharedPtrTest.cc" "${LIBRARIES}")

