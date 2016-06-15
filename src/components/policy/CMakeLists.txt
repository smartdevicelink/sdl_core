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

include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/platform.cmake)
include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/sources.cmake)

find_package(SDLSqlite3 REQUIRED)

include_directories(
  ${COMPONENTS_DIR}/include
  ${COMPONENTS_DIR}/policy/include
  ${COMPONENTS_DIR}/rpc_base/include
  ${COMPONENTS_DIR}/utils/include/
  ${COMPONENTS_DIR}/config_profile/include
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${SQLITE3_INCLUDE_DIRECTORY}
)

set(POLICY_TABLE_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/policy/policy_table
  ${CMAKE_CURRENT_SOURCE_DIR}/src/policy_table
)
collect_sources(POLICY_TABLE_SOURCES "${POLICY_TABLE_PATHS}")

set(USAGE_STATISTICS_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/policy/usage_statistics
  ${CMAKE_CURRENT_SOURCE_DIR}/src/usage_statistics
)
collect_sources(USAGE_STATISTICS_SOURCES "${USAGE_STATISTICS_PATHS}")

set(EXCLUDE_PATHS
  ${POLICY_TABLE_PATHS}
  ${USAGE_STATISTICS_PATHS}
)

set(PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/
  ${CMAKE_CURRENT_SOURCE_DIR}/src/
)
collect_sources(SOURCES "${PATHS}" "${EXCLUDE_PATHS}")

set(LIBRARIES
  config_profile
  dbms
  jsoncpp
  Utils
  ${SQLITE3_LIBRARIES}
)

if(LOG4CXX_LOGGER)
  list(APPEND LIBRARIES log4cxx)
endif()

add_library(policy_table ${POLICY_TABLE_SOURCES})
target_link_libraries(policy_table ${LIBRARIES})

add_library(usage_statistics ${USAGE_STATISTICS_SOURCES})
target_link_libraries(usage_statistics ${LIBRARIES})

list(APPEND LIBRARIES
  policy_table
  usage_statistics
)

add_library(Policy SHARED ${SOURCES})
target_link_libraries(Policy ${LIBRARIES})

set(LIBRARY_NAME ${CMAKE_SHARED_LIBRARY_PREFIX}Policy${CMAKE_SHARED_LIBRARY_SUFFIX})

get_os(OS)
get_sdk(SDK)
if(${OS} STREQUAL "win" AND NOT SDK)
  set(TARGET_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/$(CONFIGURATION)/${LIBRARY_NAME})
else()
  set(TARGET_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME})
endif()

set(INSTALL_DESTINATION bin)
set(COPY_DESTINATION ${CMAKE_BINARY_DIR}/src/appMain)

add_custom_target(copy_policy_library ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${TARGET_LOCATION}
    ${COPY_DESTINATION}/${LIBRARY_NAME}
    DEPENDS Policy
    COMMENT "Copying library ${LIBRARY_NAME}"
)

install(TARGETS Policy
  DESTINATION ${INSTALL_DESTINATION}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)

if(BUILD_TESTS)
  add_library(Policy_static ${SOURCES})
  target_link_libraries(Policy_static ${LIBRARIES})
  add_subdirectory(test)
endif()
