include_directories(
  include
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${CMAKE_SOURCE_DIR}/src/components/policy/usage_statistics/include
  ${GMOCK_INCLUDE_DIRECTORY}
)

set(LIBRARIES
  gmock_main
  UsageStatistics
)

set(SOURCES
  src/test_counter.cc
)

create_test("test_counter" "${SOURCES}" "${LIBRARIES}")
