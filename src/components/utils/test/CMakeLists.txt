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

set(UTILS_TEST_DIR ${COMPONENTS_DIR}/utils/test)

find_package(SDLSqlite3 REQUIRED)

include_directories (
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ${COMPONENTS_DIR}/utils/include
  ${COMPONENTS_DIR}/policy/include
  ${COMPONENTS_DIR}/rpc_base/include
  ${COMPONENTS_DIR}/utils/test/include
  ${SQLITE3_INCLUDE_DIRECTORY}
)

set(testSources
  messagemeter_test.cc
  file_system_test.cc
  date_time_test.cc
  thread_validator_test.cc
  bitstream_test.cc
  prioritized_queue_test.cc
  data_accessor_test.cc
  stl_utils_test.cc
  async_runner_test.cc
  shared_ptr_test.cc
  scope_guard_test.cc
  atomic_object_test.cc
  message_loop_thread_test.cc
  custom_string_test.cc
  timer_test.cc
  lock_test.cc
  thread_test.cc
)

if (ENABLE_LOG)
  list(APPEND testSources auto_trace_test.cc)
endif()

if (BUILD_BACKTRACE_SUPPORT)
  list(APPEND testSources back_trace_test.cc)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  list (APPEND testSources
    system_test.cc
    conditional_variable_test.cc
    message_queue_test.cc
    singleton_test.cc
    rwlock_posix_test.cc
    resource_usage_test.cc
  )

endif()

set(testLibraries
  gmock
  Utils
  Policy
  config_profile
)

if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
  # --- Tests for QDB Wrapper
  list (APPEND testSources
    ${UTILS_TEST_DIR}/qdb_wrapper/sql_database_test.cc
    ${UTILS_TEST_DIR}/qdb_wrapper/sql_query_test.cc
  )
  file(COPY qdbserver.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY test-qdb.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY policy.sql DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
elseif(NOT QT_PORT)
  # --- Tests for SQLite Wrapper
  list (APPEND testSources
    ${UTILS_TEST_DIR}/sqlite_wrapper/sql_database_test.cc
    ${UTILS_TEST_DIR}/sqlite_wrapper/sql_query_test.cc
    ${UTILS_TEST_DIR}/generated_code_with_sqlite_test.cc
  )
endif()

file(COPY testscript.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY log4cxx.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY smartDeviceLink.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

create_test("utils_test" "${testSources}" "${testLibraries}")

add_subdirectory(test_generator)
