find_package(Sqlite3 REQUIRED)

include_directories(
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ${CMAKE_SOURCE_DIR}/src/components/policy/src/policy/sqlite_wrapper/include
)

set(LIBRARIES
  gtest
  gtest_main
  gmock
  gmock_main
  dbms
  sqlite3
)

set(DATABASE_SOURCES
  ./src/test_sql_database.cc
)

set(QUERY_SOURCES
  ./src/test_sql_query.cc
)

create_test("test_SQLDatabase" "${DATABASE_SOURCES}" "${LIBRARIES}")
create_test("test_SQLQuery" "${QUERY_SOURCES}" "${LIBRARIES}")
