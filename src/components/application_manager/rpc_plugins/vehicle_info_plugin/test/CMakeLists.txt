# Copyright (c) 2018, Ford Motor Company
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

include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/sources.cmake)

include_directories(
  include
  ${GMOCK_INCLUDE_DIRECTORY}
  ${COMPONENTS_DIR}/application_manager/rpc_plugins/vehicle_info_plugin/include
  ${COMPONENTS_DIR}/application_manager/rpc_plugins/vehicle_info_plugin/include/vehicle_info_plugin/commands
  ${COMPONENTS_DIR}/application_manager/test/include/
  ${COMPONENTS_DIR}/include/test/application_manager/
  ${COMPONENTS_DIR}/resumption/include/resumption/
  )
set(COMMANDS_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/commands)

file(GLOB SOURCES
  ${COMPONENTS_DIR}/application_manager/test/mock_message_helper.cc
  ${COMPONENTS_DIR}/application_manager/test/mock_application_helper.cc
  ${COMPONENTS_DIR}/application_manager/src/smart_object_keys.cc
  ${COMPONENTS_DIR}/${POLICY_PATH}/src/policy_table/types.cc
  ${COMPONENTS_DIR}/${POLICY_PATH}/src/policy_table/validation.cc
  ${COMPONENTS_DIR}/${POLICY_PATH}/src/policy_table/enums.cc
  ${COMMANDS_TEST_DIR}/hmi/*
  ${COMMANDS_TEST_DIR}/mobile/*
  ${CMAKE_CURRENT_SOURCE_DIR}/custom_vehicle_data_manager_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/vehicle_data_item_schema_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/vehicle_info_pending_resumption_test.cc
  ${COMPONENTS_DIR}/application_manager/src/message.cc
  ${COMPONENTS_DIR}/application_manager/src/event_engine/*
  ${COMPONENTS_DIR}/resumption/src/last_state_wrapper_impl.cc
)

set(LIBRARIES
  gmock
  Utils
  HMI_API
  MOBILE_API
  connectionHandler
  vehicle_info_plugin_static
  SmartObjects
  formatters
  jsoncpp
)

create_cotired_test("vi_commands_test" "${SOURCES}" "${LIBRARIES}" )
