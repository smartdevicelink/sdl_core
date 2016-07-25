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

include_directories(
  ${COMPONENTS_DIR}/application_manager/include/application_manager/commands
  ${COMPONENTS_DIR}/application_manager/test/include/
  ${COMPONENTS_DIR}/application_manager/test/include/application_manager
  ${GMOCK_INCLUDE_DIRECTORY}
)

#=============== COMMANDS =====================
set(COMMANDS_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/commands
)
collect_sources(COMMANDS_SOURCES "${COMMANDS_PATHS}")

#=============== MESSAGE HELPER =====================
set(MESSAGE_HELPER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/message_helper/message_helper_test.cc
)

#=============== MOCK MESSAGE HELPER LIB =====================
set(MOCK_MESSAGE_HELPER_SOURCE
  ${CMAKE_CURRENT_SOURCE_DIR}/message_helper/mock_message_helper.cc
)

#=============== REQUEST CONTROLLER =====================
set(REQUEST_CONTROLLER_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/request_controller
)
# Temporary exclude some resumption tests from build due to compile errors APPLINK-26346
set(REQUEST_CONTROLLER_EXCLUDE
  request_controller_test.cc
  zero_request_amount_test.cc
)
collect_sources(REQUEST_CONTROLLER_SOURCES "${REQUEST_CONTROLLER_PATHS}" "${REQUEST_CONTROLLER_EXCLUDE}")

#=============== RESUMPTION =====================
set(RESUMPTION_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/resumption
)
# Temporary exclude some resumption tests from build due to compile errors APPLINK-26346
set(RESUMPTION_EXCLUDE
  resumption_data_json_test.cc
  resumption_data_db_test.cc
)

get_sdk(SDK)
if(SDK)
  if(${SDK} STREQUAL "qt")
    # Temporary exclude some resumption tests from build due to compile errors APPLINK-26346
    list(APPEND RESUMPTION_EXCLUDE resumption_data_test.cc)
    list(APPEND RESUMPTION_EXCLUDE resumption_sql_queries_test.cc)
  endif()
endif()

collect_sources(RESUMPTION_SOURCES "${RESUMPTION_PATHS}" "${RESUMPTION_EXCLUDE}")

#=============== STATE CONTROLLER =====================
set(STATE_CONTROLLER_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/state_controller
)
collect_sources(STATE_CONTROLLER_SOURCES "${STATE_CONTROLLER_PATHS}")

#=============== MOBILE MESSAGE HANDLER=====================
set(MOBILE_MESSAGE_HANDLER_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/mobile_message_handler
)
collect_sources(MOBILE_MESSAGE_HANDLER_SOURCES "${MOBILE_MESSAGE_HANDLER_PATHS}")

#=============== APPLICATION =====================
set(APPLICATION_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/application_impl/
)
collect_sources(APPLICATION_SOURCES "${APPLICATION_PATHS}")

#=============== APPLICATION_MANAGER =====================
set(APPLICATION_MANAGER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/application_manager_impl_test.cc
)

#=============== EVENT ENGINE =====================
set(AM_EVENT_ENGINE_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/event_engine/event_engine_test.cc
)
collect_sources(AM_EVENT_ENGINE_SOURCES "${AM_EVENT_ENGINE_PATHS}")

#=============== HMI CAPABILITIES =====================
set(AM_HMI_CAPABILITIES_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/hmi_capabilities_test.cc
)

#=============== HMI LANGUAGE HANDLER =====================
set(AM_HMI_LANGUAGE_HANDLER
  ${CMAKE_CURRENT_SOURCE_DIR}/hmi_language_handler_test.cc
)

##=============== POLICIES =====================
set(AM_POLICIES_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/policies
)
collect_sources(AM_POLICIES_SOURCES "${AM_POLICIES_PATHS}")

set(LIBRARIES
  gmock
  HMI_API
  MOBILE_API
  application_manager
  am_message_helper
  am_policies
  am_resumption
  connection_handler
  protocol_handler
  usage_statistics
  smart_object
  formatters
  utils
  resumption
  jsoncpp
)

add_library(mock_message_helper_library ${MOCK_MESSAGE_HELPER_SOURCE})

create_test(application_manager_test "${APPLICATION_MANAGER_SOURCES}" "${LIBRARIES}")
create_test(am_event_engine_test "${AM_EVENT_ENGINE_SOURCES}" "${LIBRARIES}")
create_test(am_mobile_handler_test "${MOBILE_MESSAGE_HANDLER_SOURCES}" "${LIBRARIES}")
create_test(am_message_helper_test "${MESSAGE_HELPER_SOURCES}" "${LIBRARIES}")
create_test(am_request_controller_test "${REQUEST_CONTROLLER_SOURCES}" "${LIBRARIES}")

list(REMOVE_ITEM LIBRARIES
  am_message_helper
)

list(APPEND LIBRARIES
  mock_message_helper_library
)

create_test(am_hmi_capabilities_test "${AM_HMI_CAPABILITIES_SOURCES}" "${LIBRARIES}")
create_test(am_application_test "${APPLICATION_SOURCES}" "${LIBRARIES}")
create_test(am_commands_test "${COMMANDS_SOURCES}" "${LIBRARIES}")
create_test(am_resumption_test "${RESUMPTION_SOURCES}" "${LIBRARIES}")
create_test(am_state_controller_test "${STATE_CONTROLLER_SOURCES}" "${LIBRARIES}")
create_test(am_hmi_language_handler_test "${AM_HMI_LANGUAGE_HANDLER}" "${LIBRARIES}")


list(APPEND LIBRARIES
  application_manager
)

create_test(am_policies_test "${AM_POLICIES_SOURCES}" "${LIBRARIES}")

set(POLICY_LIBRARY_NAME ${CMAKE_SHARED_LIBRARY_PREFIX}Policy${CMAKE_SHARED_LIBRARY_SUFFIX})
get_os(OS)
get_sdk(SDK)
if(${OS} STREQUAL "win" AND NOT SDK)
  set(POLICY_LIBRARY_PATH ${CMAKE_BINARY_DIR}/src/components/policy/$(CONFIGURATION)/${POLICY_LIBRARY_NAME})
else()
  set(POLICY_LIBRARY_PATH ${CMAKE_BINARY_DIR}/src/components/policy/${POLICY_LIBRARY_NAME})
endif()

add_custom_target(am_test_copy_policy_library ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${POLICY_LIBRARY_PATH}
    ${CMAKE_CURRENT_BINARY_DIR}/${POLICY_LIBRARY_NAME}
    DEPENDS Policy
    COMMENT "Copying library ${POLICY_LIBRARY_NAME}"
)

file(COPY smartDeviceLink_test2.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sdl_preloaded_pt.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sdl_pt_update.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY hmi_capabilities.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY smartDeviceLink_test.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resumption)
file(COPY smartDeviceLink_test.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resumption)
