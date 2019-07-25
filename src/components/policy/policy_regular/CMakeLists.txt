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
include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/generators.cmake)

include_directories (
  ${POLICY_PATH}/include/
  ${COMPONENTS_DIR}/
  ${COMPONENTS_DIR}/utils/include/
  ${COMPONENTS_DIR}/config_profile/include
  ${COMPONENTS_DIR}/connection_handler/include/
  ${COMPONENTS_DIR}/protocol_handler/include/
  ${COMPONENTS_DIR}/config_profile/include/
  ${COMPONENTS_DIR}/smart_objects/include/
  ${COMPONENTS_DIR}/rpc_base/include/
  ${CMAKE_BINARY_DIR}/src/components/
  ${POLICY_GLOBAL_INCLUDE_PATH}/
  ${BOOST_INCLUDE_DIR}/
  ${JSONCPP_INCLUDE_DIRECTORY}/
  ${LOG4CXX_INCLUDE_DIRECTORY}/
)

set(GENERATED_MOBILE_POLICY_TYPES_CPP
  "${CMAKE_CURRENT_BINARY_DIR}/generated_MOBILE_API_policy_types.cc"
  "${CMAKE_CURRENT_BINARY_DIR}/generated_MOBILE_API_enum_schema_factory.cc")

set(GENERATED_MOBILE_POLICY_TYPES_HPP
  "${CMAKE_CURRENT_BINARY_DIR}/generated_MOBILE_API_policy_types.h")

set(GENERATED_MOBILE_POLICY_TYPES
  ${GENERATED_MOBILE_POLICY_TYPES_HPP}
  ${GENERATED_MOBILE_POLICY_TYPES_CPP})

generate_policy_types("${GENERATED_MOBILE_POLICY_TYPES}"
                      "${COMPONENTS_DIR}/interfaces/MOBILE_API.xml"
                      "rpc::policy_table_interface_base"
                      "mobile-policy-types")


set(GENERATED_HMI_POLICY_TYPES_CPP
  "${CMAKE_CURRENT_BINARY_DIR}/generated_HMI_API_policy_types.cc")

set(GENERATED_HMI_POLICY_TYPES_HPP
  "${CMAKE_CURRENT_BINARY_DIR}/generated_HMI_API_policy_types.h")

set(GENERATED_HMI_POLICY_TYPES
  ${GENERATED_HMI_POLICY_TYPES_HPP}
  ${GENERATED_HMI_POLICY_TYPES_CPP})


generate_policy_types("${GENERATED_HMI_POLICY_TYPES}"
                    "${COMPONENTS_DIR}/interfaces/HMI_API.xml"
                    "rpc::policy_table_interface_base"
                    "hmi-policy-types")

set(POLICY_TABLE_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/policy/policy_table
  ${CMAKE_CURRENT_SOURCE_DIR}/src/policy_table
  ${CMAKE_CURRENT_BINARY_DIR}/
)

set(POLICY_TABLE_SOURCES
  ${GENERATED_MOBILE_POLICY_TYPES_CPP}
  ${GENERATED_HMI_POLICY_TYPES_CPP}
  ${POLICY_PATH}/src/policy_table/enums.cc
  ${POLICY_PATH}/src/policy_table/types.cc
  ${POLICY_PATH}/src/policy_table/validation.cc
)

add_library(policy_struct ${POLICY_TABLE_SOURCES})
target_link_libraries(policy_struct
  MOBILE_API
  HMI_API
  Utils
  SmartObjects
  formatters
  jsoncpp
)

set(USAGE_STATISTICS_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/policy/usage_statistics
  ${CMAKE_CURRENT_SOURCE_DIR}/src/usage_statistics
)
collect_sources(USAGE_STATISTICS_SOURCES "${USAGE_STATISTICS_PATHS}")
add_library(UsageStatistics ${USAGE_STATISTICS_SOURCES})
target_link_libraries(UsageStatistics Utils)

set(EXCLUDE_PATHS
  ${POLICY_TABLE_PATHS}
  ${USAGE_STATISTICS_PATHS}
)

set(PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${COMPONENTS_DIR}/rpc_base/src/rpc_base/rpc_base.cc
)
collect_sources(SOURCES "${PATHS}" "${EXCLUDE_PATHS}")

set(LIBRARIES
  ConfigProfile
  policy_struct
  dbms
  Utils
)

if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
  # --- QDB Wrapper 
  include_directories (${COMPONENTS_DIR}/utils/include/utils)
else ()
  # --- SQLite Wrapper
  include_directories (${COMPONENTS_DIR}/utils/include/utils)
  list(APPEND LIBRARIES sqlite3)
endif ()


add_library(PolicyStatic ${SOURCES})
target_link_libraries(PolicyStatic ${LIBRARIES})


add_library(Policy SHARED "src/policy_manager_impl.cc")
target_link_libraries(Policy PolicyStatic)

if(ENABLE_LOG)
  target_link_libraries(Policy log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

set(LIBRARY_NAME ${CMAKE_SHARED_LIBRARY_PREFIX}Policy${CMAKE_SHARED_LIBRARY_SUFFIX})
set(TARGET_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME})
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
  add_subdirectory(test)
endif()
