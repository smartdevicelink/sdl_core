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

include_directories(
  include
  ${COMPONENTS_DIR}
  ${GMOCK_INCLUDE_DIRECTORY}
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${COMPONENTS_DIR}/policy/src/policy/include/
  ${COMPONENTS_DIR}/policy/src/policy/usage_statistics/include
  ${COMPONENTS_DIR}/rpc_base/include
  ${COMPONENTS_DIR}/config_profile/include
  ${COMPONENTS_DIR}/utils/include/
  ${COMPONENTS_DIR}/policy/src/policy/policy_table/table_struct
  ${COMPONENTS_DIR}/policy/src/policy/policy_table/
)

set(testLibraries
  gmock
  Utils
  Policy
  UsageStatistics
)

set(testSources
  usage_statistics_test.cc
  shared_library_test.cc
  generated_code_test.cc
  policy_manager_impl_test.cc
  update_status_manager_test.cc
)

list (APPEND testSources
 sql_pt_representation_test.cc
)

if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
  list(REMOVE_ITEM testLibraries dl)
  # --- Tests for QDB Wrapper
  include_directories(../../utils/include/utils)
  list (APPEND testSources
        ../../utils/test/qdb_wrapper/sql_database_test.cc
        ../../utils/test/qdb_wrapper/sql_query_test.cc
  )
  file(COPY qdbserver.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY test-qdb.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY policy.sql DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
else ()
  # --- Tests for SQLite Wrapper
  find_package(Sqlite3 REQUIRED)
  include_directories(../../utils/include/utils
                      ../../utils/test/include )
  list (APPEND testSources
#    ../../utils/test/sqlite_wrapper/sql_database_test.cc
#    ../../utils/test/sqlite_wrapper/sql_query_test.cc
#    ../../utils/test/generated_code_with_sqlite_test.cc
#   policy_manager_impl_stress_test.cc
  )
  list (APPEND testLibraries sqlite3)
endif()

create_test("policy_test" "${testSources}" "${testLibraries}")

file(COPY valid_sdl_pt_update.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sdl_preloaded_pt.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY PTU.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY PTU2.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY PTU3.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY PTU4.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ptu_requestType.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ptu2_requestType.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY log4cxx.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY smartDeviceLink.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY smartDeviceLink2.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY smartDeviceLink3.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sdl_pt_first_update.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sdl_pt_second_update.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sdl_pt_update.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
