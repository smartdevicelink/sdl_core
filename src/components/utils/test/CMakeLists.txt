# Copyright (c) 2015, Ford Motor Company
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

if(BUILD_TESTS)

include_directories (
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${CMAKE_SOURCE_DIR}/src/3rd_party-static/gmock-1.7.0/include
  ${CMAKE_SOURCE_DIR}/src/3rd_party-static/gmock-1.7.0/gtest/include
  ${COMPONENTS_DIR}/utils/include/utils
  ${COMPONENTS_DIR}/include/utils
  ${COMPONENTS_DIR}/rpc_base/include
  ${COMPONENTS_DIR}/utils/test/include
)

set(testSources
  messagemeter_test.cc
  file_system_test.cc
  date_time_test.cc
  system_test.cc
  thread_validator_test.cc
  conditional_variable_test.cc
  message_queue_test.cc
  resource_usage_test.cc
  bitstream_test.cc
  prioritized_queue_test.cc
  data_accessor_test.cc
  lock_posix_test.cc
  singleton_test.cc
  #posix_thread_test.cc
  stl_utils_test.cc
  rwlock_posix_test.cc
  async_runner_test.cc
  shared_ptr_test.cc
  scope_guard_test.cc
  atomic_object_test.cc
  message_loop_thread_test.cc
  custom_string_test.cc
  timer_test.cc
)

set(testLibraries
  gmock
  Utils
  Policy
  ConfigProfile
)

include_directories(${COMPONENTS_DIR}/policy/src/policy/policy_table/table_struct)

if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
  # --- Tests for QDB Wrapper
  list (APPEND testSources
    ./qdb_wrapper/sql_database_test.cc
    ./qdb_wrapper/sql_query_test.cc
  )
  file(COPY qdbserver.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY test-qdb.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY policy.sql DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
else ()
  # --- Tests for SQLite Wrapper
  find_package(Sqlite3 REQUIRED)
  list (APPEND testSources
    ./sqlite_wrapper/sql_database_test.cc
    ./sqlite_wrapper/sql_query_test.cc
    generated_code_with_sqlite_test.cc
  )
  list (APPEND testLibraries sqlite3)
endif()

if (ENABLE_LOG)
  list(APPEND testSources auto_trace_test.cc)
  list(APPEND testSources log_message_loop_thread_test.cc)
endif()

if (BUILD_BACKTRACE_SUPPORT)
  list(APPEND testSources back_trace_test.cc)
endif()

file(COPY testscript.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY log4cxx.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY smartDeviceLink.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

create_test("utils_test" "${testSources}" "${testLibraries}")

endif()
