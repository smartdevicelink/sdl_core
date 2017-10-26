# Copyright (c) 2016, Ford Motor Company
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

# --- Policy
set(target Policy)
set(install_destination bin)
set(copy_destination ${CMAKE_BINARY_DIR}/src/appMain)
set(library_name ${CMAKE_SHARED_LIBRARY_PREFIX}${target}${CMAKE_SHARED_LIBRARY_SUFFIX})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include_directories (
  include
  ${COMPONENTS_DIR}/rpc_base/include
  ${COMPONENTS_DIR}/utils/include/
  ${COMPONENTS_DIR}/config_profile/include
  ${POLICY_GLOBAL_INCLUDE_PATH}/
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${JSONCPP_INCLUDE_DIRECTORY}
)

set(SOURCES
  ${POLICY_PATH}/src/policy_manager_impl.cc
  ${POLICY_PATH}/src/policy_helper.cc
  ${POLICY_PATH}/src/policy_table.cc
  ${POLICY_PATH}/src/sql_pt_queries.cc
  ${POLICY_PATH}/src/sql_pt_representation.cc
  ${POLICY_PATH}/src/update_status_manager.cc
  ${POLICY_PATH}/src/status.cc
  ${POLICY_PATH}/src/cache_manager.cc
  ${COMPONENTS_DIR}/rpc_base/src/rpc_base/rpc_base.cc
)

if (REMOTE_CONTROL)
  list(APPEND SOURCES ${POLICY_PATH}/src/access_remote_impl.cc)
endif()

list(APPEND SOURCES
  ${POLICY_PATH}/src/sql_pt_ext_queries.cc
  ${POLICY_PATH}/src/sql_pt_ext_representation.cc
)

set(LIBRARIES ConfigProfile policy_struct dbms jsoncpp Utils)
if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
  # --- QDB Wrapper
  include_directories (${COMPONENTS_DIR}/utils/include/utils)
else ()
  # --- SQLite Wrapper
  include_directories (${COMPONENTS_DIR}/utils/include/utils)
  list(APPEND LIBRARIES sqlite3)
endif ()

add_library(${target} SHARED ${SOURCES})
target_link_libraries(${target} ${LIBRARIES} )

if (ENABLE_LOG)
  target_link_libraries(${target} log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

add_custom_target(copy_library_${target} ALL
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/${library_name}
    ${copy_destination}
    DEPENDS ${target}
    COMMENT "Copying library ${library_name}")

install(TARGETS ${target}
  DESTINATION ${install_destination}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)

# --- Table struct
set(struct_target policy_struct)

set(TABLE_STRUCT_SOURCES
  ${POLICY_PATH}/src/policy_table/enums.cc
  ${POLICY_PATH}/src/policy_table/types.cc
  ${POLICY_PATH}/src/policy_table/validation.cc
)

add_library(${struct_target} ${TABLE_STRUCT_SOURCES})
target_link_libraries(${struct_target} Utils)

# --- Usage statistics
set(USAGE_STATISTICS_SOURCES
  ./src/usage_statistics/counter.cc
)

add_library(UsageStatistics ${USAGE_STATISTICS_SOURCES})
target_link_libraries(UsageStatistics Utils)

#======================= Unit-Test section =======================
if (BUILD_TESTS)
  add_subdirectory(test)
endif()
#=================================================================
