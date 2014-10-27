# Copyright (c) 2014, Ford Motor Company
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following
# disclaimer in the documentation and/or other materials provided with the
# distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

set(POLICY_DIR ${CMAKE_SOURCE_DIR}/src/components/policy)

include_directories(
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${POLICY_DIR}/test/include
  ${POLICY_DIR}/src/policy/include/
  ${POLICY_DIR}/src/policy/sqlite_wrapper/include
  ${POLICY_DIR}/src/policy/qdb_wrapper/include
  ${POLICY_DIR}/src/policy/usage_statistics/include
  ${CMAKE_SOURCE_DIR}/src/components/rpc_base/include
  ${CMAKE_SOURCE_DIR}/src/components/utils/include/
)

list(APPEND test_exec_libraries
  Policy
  UsageStatistics
  dbms
  Utils
  dl
  ${RTLIB}
)

list(APPEND testSources
  ${POLICY_DIR}/test/usage_statistics_test.cc
  ${POLICY_DIR}/test/generated_code_test.cc
  ${POLICY_DIR}/test/policy_manager_impl_test.cc
  # TODO(KKolodiy): need to resolve issue about path to libPolicy.so
  # ${POLICY_DIR}/test/shared_library_test.cc
)

if (EXTENDED_POLICY_FLAG)
  add_definitions(-DEXTENDED_POLICY)
  include_directories(${POLICY_DIR}/src/policy/policy_table/table_struct_ext)
  list (APPEND testSources ${POLICY_DIR}/test/sql_pt_ext_representation_test.cc)
else ()
  include_directories(${POLICY_DIR}/src/policy/policy_table/table_struct)
  list (APPEND testSources ${POLICY_DIR}/test/sql_pt_representation_test.cc)
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
  list(REMOVE_ITEM test_exec_libraries dl)
  # --- Tests for QDB Wrapper
  include_directories(${POLICY_DIR}/src/policy/qdb_wrapper/include)
  list (APPEND testSources
    ${POLICY_DIR}/test/qdb_wrapper/sql_database_test.cc
    ${POLICY_DIR}/test/qdb_wrapper/sql_query_test.cc
  )
  file(COPY ${POLICY_DIR}/test/qdbserver.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY ${POLICY_DIR}/test/test-qdb.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY ${POLICY_DIR}/test/policy.sql DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
else ()
  # --- Tests for SQLite Wrapper
  find_package(Sqlite3 REQUIRED)
  include_directories(${POLICY_DIR}/src/policy/sqlite_wrapper/include)
  list (APPEND testSources
    ${POLICY_DIR}/test/sqlite_wrapper/sql_database_test.cc
    ${POLICY_DIR}/test/sqlite_wrapper/sql_query_test.cc
    ${POLICY_DIR}/test/generated_code_with_sqlite_test.cc
    ${POLICY_DIR}/test/policy_manager_impl_stress_test.cc
  )
  list (APPEND test_exec_libraries sqlite3)
endif()

file(COPY ${POLICY_DIR}/test/valid_sdl_pt_update.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${POLICY_DIR}/test/sdl_preloaded_pt.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})