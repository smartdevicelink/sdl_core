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

include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/sources.cmake)

include_directories(
  ${COMPONENTS_DIR}/application_manager/include/application_manager/commands
  ${COMPONENTS_DIR}/application_manager/test/include/
  ${COMPONENTS_DIR}/application_manager/test/include/application_manager
  ${GMOCK_INCLUDE_DIRECTORY}
)

set(COMMANDS_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/commands
)
collect_sources(COMMANDS_SOURCES "${COMMANDS_PATHS}")

list(APPEND COMMANDS_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_message_helper.cc
)

set(MESSAGE_HELPER_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/message_helper
)
collect_sources(MESSAGE_HELPER_SOURCES "${MESSAGE_HELPER_PATHS}")

set(REQUEST_CONTROLLER_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/request_controller
)
# Temporary exclude some resumption tests from build due to compile errors
set(REQUEST_CONTROLLER_EXCLUDE
  request_controller_test.cc
)
collect_sources(REQUEST_CONTROLLER_SOURCES "${REQUEST_CONTROLLER_PATHS}" "${REQUEST_CONTROLLER_EXCLUDE}")

set(RESUMPTION_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/resumption
)
# Temporary exclude some resumption tests from build due to compile errors
set(RESUMPTION_EXCLUDE
  resumption_data_json_test.cc
  resumption_data_db_test.cc
)

get_sdk(SDK)
if(SDK)
  if(${SDK} STREQUAL "qt")
    list(APPEND RESUMPTION_EXCLUDE resumption_data_test.cc)
  endif()
endif()

collect_sources(RESUMPTION_SOURCES "${RESUMPTION_PATHS}" "${RESUMPTION_EXCLUDE}")

list(APPEND RESUMPTION_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_message_helper.cc
)

set(STATE_CONTROLLER_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/state_controller
)
collect_sources(STATE_CONTROLLER_SOURCES "${STATE_CONTROLLER_PATHS}")

list(APPEND STATE_CONTROLLER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/mock_message_helper.cc
)

set(EXCLUDE_PATHS
  ${COMMANDS_PATHS}
  ${MESSAGE_HELPER_PATHS}
  ${REQUEST_CONTROLLER_PATHS}
  ${RESUMPTION_PATHS}
  ${STATE_CONTROLLER_PATHS}
)

set(PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}
)
# Temporary exclude some resumption tests from build due to compile errors
list(APPEND EXCLUDE_PATHS
  zero_request_amount_test.cc
)
collect_sources(SOURCES "${PATHS}" "${EXCLUDE_PATHS}")

set(LIBRARIES
  gmock
  HMI_API
  MOBILE_API
  application_manager
  connection_handler
  protocol_handler
  usage_statistics
  formatters
  Utils
  resumption
  jsoncpp
)

create_test(am_commands_test "${COMMANDS_SOURCES}" "${LIBRARIES}")
create_test(am_message_helper_test "${MESSAGE_HELPER_SOURCES}" "${LIBRARIES}")
create_test(am_request_controller_test "${REQUEST_CONTROLLER_SOURCES}" "${LIBRARIES}")
create_test(am_resumption_test "${RESUMPTION_SOURCES}" "${LIBRARIES}")
create_test(am_state_controller_test "${STATE_CONTROLLER_SOURCES}" "${LIBRARIES}")
create_test(application_manager_test "${SOURCES}" "${LIBRARIES}")

set(COPY_DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
set(LIBRARY_NAME ${CMAKE_SHARED_LIBRARY_PREFIX}Policy${CMAKE_SHARED_LIBRARY_SUFFIX})

add_custom_target(am_test_copy_policy_library ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/src/appMain/${LIBRARY_NAME}
    ${COPY_DESTINATION}/${LIBRARY_NAME}
    DEPENDS copy_policy_library
    COMMENT "Copying library ${LIBRARY_NAME}"
)

file(COPY smartDeviceLink_test2.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sdl_preloaded_pt.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sdl_pt_update.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY hmi_capabilities.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY smartDeviceLink_test.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resumption)
file(COPY smartDeviceLink_test.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resumption)
