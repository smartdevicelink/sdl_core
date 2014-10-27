# Copyright (c) 2013-2014, Ford Motor Company
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

include_directories(
  include
  ${GMOCK_INCLUDE_DIRECTORY}
  ${JSONCPP_INCLUDE_DIRECTORY}
  ../src/policy/include/
  ../src/policy/sqlite_wrapper/include
  ../src/policy/qdb_wrapper/include
  ../src/policy/usage_statistics/include
  ${CMAKE_SOURCE_DIR}/src/components/rpc_base/include
  ${CMAKE_SOURCE_DIR}/src/components/utils/include/
)

set(testLibraries
  gtest
  gmock
  Utils
  Policy
  ${RTLIB}
  dl
  UsageStatistics
  dbms
)

set(testSources
  main.cc
  usage_statistics_test.cc
  shared_library_test.cc
  generated_code_test.cc
  policy_manager_impl_test.cc
)

  include_directories(../src/policy/policy_table/table_struct)
  list (APPEND testSources
    sql_pt_representation_test.cc
  )

if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
  list(REMOVE_ITEM testLibraries dl)
  # --- Tests for QDB Wrapper
  include_directories(../src/policy/qdb_wrapper/include)
  list (APPEND testSources
    qdb_wrapper/sql_database_test.cc
    qdb_wrapper/sql_query_test.cc
  )
  file(COPY qdbserver.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY test-qdb.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY policy.sql DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
else ()
  # --- Tests for SQLite Wrapper
  find_package(Sqlite3 REQUIRED)
  include_directories(../src/policy/sqlite_wrapper/include)
  list (APPEND testSources
    sqlite_wrapper/sql_database_test.cc
    sqlite_wrapper/sql_query_test.cc
    generated_code_with_sqlite_test.cc
    policy_manager_impl_stress_test.cc
  )
  list (APPEND testLibraries sqlite3)
endif()

add_executable(policy_test ${testSources})
target_link_libraries(policy_test ${testLibraries})

file(COPY valid_sdl_pt_update.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sdl_preloaded_pt.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY log4cxx.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
