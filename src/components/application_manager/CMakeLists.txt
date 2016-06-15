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

find_package(SDLOpenSSL REQUIRED)
find_package(SDLSqlite3 REQUIRED)

include_directories(
  ${COMPONENTS_DIR}/include
  ${COMPONENTS_DIR}/application_manager/include
  ${COMPONENTS_DIR}/utils/include
  ${COMPONENTS_DIR}/formatters/include
  ${COMPONENTS_DIR}/protocol_handler/include
  ${COMPONENTS_DIR}/smart_objects/include
  ${COMPONENTS_DIR}/hmi_message_handler/include
  ${COMPONENTS_DIR}/media_manager/include
  ${COMPONENTS_DIR}/connection_handler/include
  ${COMPONENTS_DIR}/config_profile/include
  ${COMPONENTS_DIR}/request_watchdog/include
  ${COMPONENTS_DIR}/resumption/include
  ${COMPONENTS_DIR}/rpc_base/include
  ${COMPONENTS_DIR}/policy/include
  ${CMAKE_BINARY_DIR}/src/components
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${OPENSSL_INCLUDE_DIRECTORY}
  ${ENCRYPTION_INCLUDE_DIRECTORY}
  ${MESSAGE_BROKER_INCLUDE_DIRECTORY}
)

set(COMMANDS_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/commands
  ${CMAKE_CURRENT_SOURCE_DIR}/src/commands
)
collect_sources(COMMANDS_SOURCES "${COMMANDS_PATHS}")

set(EVENT_ENGINE_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/application_manager/event_engine
  ${CMAKE_CURRENT_SOURCE_DIR}/src/event_engine
)
collect_sources(EVENT_ENGINE_SOURCES "${EVENT_ENGINE_PATHS}")

set(POLICIES_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/application_manager/policies
  ${CMAKE_CURRENT_SOURCE_DIR}/src/policies
)
collect_sources(POLICIES_SOURCES "${POLICIES_PATHS}")

set(RESUMPTION_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/application_manager/resumption
  ${CMAKE_CURRENT_SOURCE_DIR}/src/resumption
)
collect_sources(RESUMPTION_SOURCES "${RESUMPTION_PATHS}")

set(EXCLUDE_PATHS
  ${COMMANDS_PATHS}
  ${EVENT_ENGINE_PATHS}
  ${POLICIES_PATHS}
  ${RESUMPTION_PATHS}
)

set(PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/
  ${CMAKE_CURRENT_SOURCE_DIR}/src/
)
collect_sources(SOURCES "${PATHS}" "${EXCLUDE_PATHS}")

set(LIBRARIES
  HMI_API
  MOBILE_API
  v4_protocol_v1_2_no_extra
  protocol
  smart_object
  usage_statistics
  formatters
  dbms
  ${SQLITE3_LIBRARIES}
)

get_os(OS)
if(${OS} STREQUAL "posix")
  list(APPEND LIBRARIES dl)
endif()

add_library(am_commands_library ${COMMANDS_SOURCES})
target_link_libraries(am_commands_library ${LIBRARIES})

add_library(am_event_engine ${EVENT_ENGINE_SOURCES})
target_link_libraries(am_event_engine ${LIBRARIES})

add_library(am_policies ${POLICIES_SOURCES} )
target_link_libraries(am_policies ${LIBRARIES})

add_library(am_resumption ${RESUMPTION_SOURCES} )
target_link_libraries(am_resumption ${LIBRARIES})

list(APPEND LIBRARIES
  am_commands_library
  am_event_engine
  am_policies
  am_resumption
)

add_library(application_manager ${SOURCES} )
target_link_libraries(application_manager ${LIBRARIES})

if(BUILD_TESTS)
  add_subdirectory(test)
endif()
