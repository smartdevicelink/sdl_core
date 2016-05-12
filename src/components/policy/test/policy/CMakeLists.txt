include_directories(
  ./
  ./include
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${CMAKE_SOURCE_DIR}/src/components/policy/src/policy/include/
  ${CMAKE_SOURCE_DIR}/src/components/rpc_base/include
  ${CMAKE_SOURCE_DIR}/src/components/policy/src/policy/sqlite_wrapper/include
  ${CMAKE_SOURCE_DIR}/src/components/policy/src/policy/qdb_wrapper/include
  ${CMAKE_SOURCE_DIR}/src/components/policy/src/policy/usage_statistics/include
  ${CMAKE_SOURCE_DIR}/src/components/utils/include/
)

set(LIBRARIES
  gtest
  gtest_main
  gmock
  gmock_main
  Policy
  ConfigProfile
  ${RTLIB}
)

set(SHARED_LIBRARY_LIBRARIES
  gtest
  gtest_main
  gmock
  gmock_main
  dl
)

if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
  list(REMOVE_ITEM SHARED_LIBRARY_LIBRARIES dl)
endif()

set(SHARED_LIBRARY_SOURCES
  ./src/test_shared_library.cc
)

set(SQL_PT_REPRESENTATION_SOURCES
  ./src/test_sql_pt_representation.cc
)

set(GENERATED_CODE_SOURCES
  ./src/generated_code_test.cc
)

set(GENERATED_CODE_WITH_SQLITE_SOURCES
  ./src/generated_code_with_sqlite_test.cc
)

set(POLICY_MANAGER_IMPL_SOURCES
  ./src/test_policy_manager_impl.cc
)

set(STRESS_POLICY_MANAGER_IMPL_SOURCES
  ./src/test_stress_policy_manager_impl.cc
)

add_subdirectory(usage_statistics)



if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
  # --- Tests for QDB Wrapper
  add_subdirectory(qdb_wrapper)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/qdbserver.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test-qdb.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
else ()
  # --- Tests for SQLite Wrapper
  add_subdirectory(sqlite_wrapper)
  create_test("test_generated_code_with_sqlite" "${GENERATED_CODE_WITH_SQLITE_SOURCES}" "${LIBRARIES}")

  include_directories(${CMAKE_SOURCE_DIR}/src/components/policy/src/policy/policy_table/table_struct)
    # TODO(KKolodiy): test crashes in QNX and failed for extended policy
    create_test("test_stress_PolicyManagerImpl" "${STRESS_POLICY_MANAGER_IMPL_SOURCES}" "${LIBRARIES}")
endif()

create_test("test_generated_policy_code" "${GENERATED_CODE_SOURCES}" "${LIBRARIES}")
create_test("test_SharedLibrary" "${SHARED_LIBRARY_SOURCES}" "${SHARED_LIBRARY_LIBRARIES}")
create_test("test_SQLPTRepresentation" "${SQL_PT_REPRESENTATION_SOURCES}" "${LIBRARIES}")
create_test("test_PolicyManagerImpl" "${POLICY_MANAGER_IMPL_SOURCES}" "${LIBRARIES}")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/valid_sdl_pt_update.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/log4cxx.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
