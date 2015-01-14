add_definitions(-DUNIT_TESTS)

# --- SchemaItems
add_subdirectory(./SchemaItem)

include_directories (
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${CMAKE_SOURCE_DIR}/src/components/utils/include
  ${GMOCK_INCLUDE_DIRECTORY}
  ../../../src/components/smart_objects/include
  ../../../src/components/formatters/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    SmartObjects
    formatters
    jsoncpp
    ${RTLIB}
    ProtocolLibrary
)

set (SOURCES
  ./SmartObjectDraftTest.cc
  ./SmartObjectInvalidTest.cc
  ./SmartObjectStressTest.cc
  ./SmartObjectUnitTest.cc
  ./TSharedPtrTest.cc
)

add_library(test_SmartObject ${SOURCES})
create_test(test_SmartObjectTest "${SOURCES}" "${LIBRARIES}")

create_test("test_SmartObject_ConvertionTimeTest" "./SmartObjectConvertionTimeTest.cc" "${LIBRARIES}")
create_test("test_SmartObject_PerformanceTest" "./smart_object_performance_test.cc" "${LIBRARIES}")
create_test("test_Map_PerformanceTest" "./map_performance_test.cc" "${LIBRARIES}")
