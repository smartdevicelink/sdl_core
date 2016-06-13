# Copyright (c) 2013, Ford Motor Company
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

set(target Policy)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  if(NOT QT_PORT)
      find_package(WinSqlite3 REQUIRED)
  endif()
else()
  find_package(Sqlite3 REQUIRED)
endif()

set(install_destination bin)
set(copy_destination ${CMAKE_BINARY_DIR}/src/appMain)
set(library_name ${CMAKE_SHARED_LIBRARY_PREFIX}${target}${CMAKE_SHARED_LIBRARY_SUFFIX})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include_directories (
  ${COMPONENTS_DIR}/policy/include
  ${COMPONENTS_DIR}/rpc_base/include
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${COMPONENTS_DIR}/utils/include/
  ${COMPONENTS_DIR}/config_profile/include
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

set(SOURCES
  ${COMPONENTS_DIR}/policy/src/policy_manager_impl.cc
  ${COMPONENTS_DIR}/policy/src/policy_helper.cc
  ${COMPONENTS_DIR}/policy/src/policy_table.cc
  ${COMPONENTS_DIR}/policy/src/sql_pt_queries.cc
  ${COMPONENTS_DIR}/policy/src/sql_pt_representation.cc
  ${COMPONENTS_DIR}/policy/src/update_status_manager.cc
  ${COMPONENTS_DIR}/policy/src/cache_manager.cc
  ${COMPONENTS_DIR}/rpc_base/src/rpc_base/rpc_base.cc
)

# --- Table struct section
set(policy_struct_target policy_struct)
set(POLICY_TABLE_SOURCES
  ${COMPONENTS_DIR}/policy/src/policy_table/enums.cc
  ${COMPONENTS_DIR}/policy/src/policy_table/types.cc
  ${COMPONENTS_DIR}/policy/src/policy_table/validation.cc
)

add_library(${policy_struct_target} ${POLICY_TABLE_SOURCES})
target_link_libraries(${policy_struct_target} Utils)
# --- end of Table struct section

# --- Usage statistics section
set(USAGE_STATISTICS_SOURCES
  ${COMPONENTS_DIR}/policy/src/usage_statistics/counter.cc
)
add_library(UsageStatistics ${USAGE_STATISTICS_SOURCES})
# --- end of Usage statistics section

set(LIBRARIES config_profile policy_struct dbms jsoncpp Utils)
if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
  # --- QDB Wrapper 
  include_directories (${COMPONENTS_DIR}/utils/include/)
else ()
  # --- SQLite Wrapper
  if (NOT QT_PORT)
    include_directories (${COMPONENTS_DIR}/utils/include/)
    list(APPEND LIBRARIES sqlite3)
  endif()
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  source_group("Header Files" FILES ${HEADERS} )
  source_group("Source Files" FILES ${SOURCES} )
  if(NOT QT_PORT)
    link_directories(${SQLITE_LIB_DIRECTORY})
  endif()
endif()

add_library(${target} SHARED ${SOURCES})
target_link_libraries(${target} ${LIBRARIES} )

if(BUILD_TESTS)
  set(TestTarget PolicyTestLib)
  add_library(${TestTarget} STATIC ${SOURCES} ${HEADERS})
  target_link_libraries(${TestTarget} ${LIBRARIES})
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  if(ENABLE_LOG)
    target_link_libraries(${target} log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
  endif()
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
add_custom_target(copy_library_${target} ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)/${library_name}
    ${copy_destination}/${library_name}
    DEPENDS ${target}
    COMMENT "Copying library ${library_name}")
 else()
add_custom_target(copy_library_${target} ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/${library_name}
    ${copy_destination}/${library_name}
    DEPENDS ${target}
    COMMENT "Copying library ${library_name}")
endif()

install(TARGETS ${target}
  DESTINATION ${install_destination}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)

if(BUILD_TESTS)
  add_subdirectory(test)
endif() # BUILD_TESTS
