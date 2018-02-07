# Copyright (c) 2016, Ford Motor Company
# All rights reserved.
#
# Copyright (c) 2017 Xevo Inc.
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
# Neither the name of the copyright holders nor the names of their contributors
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

set (AM_SOURCE_DIR ${COMPONENTS_DIR}/application_manager)
set (AM_TEST_DIR ${AM_SOURCE_DIR}/test)
set (AM_MOCK_DIR ${AM_TEST_DIR}/mock)

include_directories (
  include/
  ${COMPONENTS_DIR}
  ${COMPONENTS_DIR}/utils/include/
  ${COMPONENTS_DIR}/formatters/include/
  ${COMPONENTS_DIR}/protocol_handler/include/
  ${COMPONENTS_DIR}/smart_objects/include/
  ${COMPONENTS_DIR}/hmi_message_handler/include
  ${COMPONENTS_DIR}/media_manager/include
  ${COMPONENTS_DIR}/connection_handler/include
  ${COMPONENTS_DIR}/config_profile/include
  ${COMPONENTS_DIR}/request_watchdog/include
  ${COMPONENTS_DIR}/resumption/include
  ${COMPONENTS_DIR}/system_time/
  ${COMPONENTS_DIR}/rpc_base/include
  ${COMPONENTS_DIR}/interfaces
  ${POLICY_PATH}/include/
  ${POLICY_GLOBAL_INCLUDE_PATH}/
  ${CMAKE_BINARY_DIR}/src/components/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${ENCRYPTION_INCLUDE_DIRECTORY}
  ${MESSAGE_BROKER_INCLUDE_DIRECTORY}
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${BSON_INCLUDE_DIRECTORY}
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  include_directories(
    ${COMPONENTS_DIR}/utils/include/utils/sqlite_wrapper/
  )
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  include_directories(
    ${COMPONENTS_DIR}/utils/include/utils/qdb_wrapper/
  )
endif()

set (MESSAGE_HELPER_SOURCE_DIR
  ${AM_SOURCE_DIR}/src/message_helper
)

set (MESSAGE_HELPER_PATHS
  ${MESSAGE_HELPER_SOURCE_DIR}
)
collect_sources(MESSAGE_HELPER_SOURCES "${MESSAGE_HELPER_PATHS}")

set (SYSTEM_TIME_SOURCES
  ${AM_SOURCE_DIR}/src/system_time/system_time_handler_impl.cc
)

set (POLICIES_SOURCE_DIR
  ${AM_SOURCE_DIR}/src/policies
)

set (POLICIES_MANAGER_SOURCES
  ${POLICIES_SOURCE_DIR}/policy_handler.cc
  ${POLICIES_SOURCE_DIR}/policy_event_observer.cc
  ${POLICIES_SOURCE_DIR}/delegates/app_permission_delegate.cc
  ${POLICIES_SOURCE_DIR}/delegates/statistics_delegate.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/usage_statistics.cc
)

include_directories(
  ${POLICY_PATH}/src/policy/policy_table/table_struct
)

set (EVENT_ENGINE_SOURCE_DIR
  ${AM_SOURCE_DIR}/src/event_engine
)

set (EVENT_ENGINE_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/application_manager/event_engine
  ${EVENT_ENGINE_SOURCE_DIR}
)
collect_sources(EVENT_ENGINE_SOURCES "${EVENT_ENGINE_PATHS}")


collect_sources(POLICIES_SOURCES "${POLICIES_PATHS}")

if (${HMI_JSON_API})
  set (HMI_COMMANDS_SOURCES ${HMI_COMMANDS_SOURCES} ${HMI_COMMANDS_SOURCES_JSON})
endif (${HMI_JSON_API})

set (HMI_COMMANDS_SOURCES ${HMI_COMMANDS_SOURCES} ${HMI_COMMANDS_SOURCES_DBUS})

set(EXCLUDE_PATHS
  ${EVENT_ENGINE_SOURCE_DIR}
  ${MESSAGE_HELPER_SOURCE_DIR}
  ${POLICIES_SOURCE_DIR}
)

set(PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
collect_sources(SOURCES "${PATHS}" "${EXCLUDE_PATHS}")

if (NOT REMOTE_CONTROL)
    list(REMOVE_ITEM SOURCES
        ${COMPONENTS_DIR}/application_manager/src/core_service.cc)
endif()

set(LIBRARIES
  HMI_API
  MOBILE_API
  v4_protocol_v1_2_no_extra
  ProtocolLibrary
  SmartObjects
  UsageStatistics
  dl
  formatters
  dbms
  Utils
  bson -L${BSON_LIBS_DIRECTORY}
  emhashmap -L${EMHASHMAP_LIBS_DIRECTORY}
)

if(REMOTE_CONTROL)
    SET (LIBRARIES
      ${LIBRARIES}
      FunctionalModule
    )
endif(REMOTE_CONTROL)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  list(APPEND LIBRARIES sqlite3)
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  list(REMOVE_ITEM LIBRARIES dl)
endif()

add_library("AMEventEngine" ${EVENT_ENGINE_SOURCES})
target_link_libraries("AMEventEngine" ${LIBRARIES})

add_library("AMPolicyLibrary" ${POLICIES_MANAGER_SOURCES} )
target_link_libraries("AMPolicyLibrary" ${LIBRARIES} AMEventEngine)

add_library("SystemTimeLibrary" ${SYSTEM_TIME_SOURCES})
target_link_libraries("SystemTimeLibrary" ${LIBRARIES} AMEventEngine)

add_library("MessageHelper" ${MESSAGE_HELPER_SOURCES})
target_link_libraries("MessageHelper" ${LIBRARIES})

add_library("ApplicationManager" ${SOURCES})

list(APPEND LIBRARIES
  AMEventEngine
  AMPolicyLibrary
)

if(ENABLE_LOG)
  list(APPEND LIBRARIES log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

add_dependencies("ApplicationManager" libbson)
target_link_libraries("ApplicationManager" ${LIBRARIES})

if(BUILD_TESTS)
  add_subdirectory(test)
endif()

add_subdirectory(rpc_plugins)
