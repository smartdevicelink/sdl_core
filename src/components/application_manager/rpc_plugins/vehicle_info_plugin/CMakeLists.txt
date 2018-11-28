# Copyright (c) 2018, Ford Motor Company All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice, this
# list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# Neither the name of the copyright holders nor the names of their contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

project(vehicle_info_plugin)

set(
  PLUGIN_SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/src/vehicle_info_app_extension.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/vehicle_info_command_factory.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/vehicle_info_hmi_command_factory.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/vehicle_info_mobile_command_factory.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/vehicle_info_plugin.cc"
  # commands/hmi
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/on_vi_vehicle_data_notification.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/vi_diagnostic_message_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/vi_diagnostic_message_response.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/vi_get_dtcs_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/vi_get_dtcs_response.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/vi_get_vehicle_data_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/vi_get_vehicle_data_response.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/vi_get_vehicle_type_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/vi_get_vehicle_type_response.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/vi_is_ready_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/vi_is_ready_response.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/vi_read_did_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/vi_read_did_response.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/vi_subscribe_vehicle_data_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/vi_subscribe_vehicle_data_response.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/vi_unsubscribe_vehicle_data_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/hmi/vi_unsubscribe_vehicle_data_response.cc"
  # commands/mobile
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/diagnostic_message_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/diagnostic_message_response.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/get_dtcs_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/get_dtcs_response.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/get_vehicle_data_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/get_vehicle_data_response.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/on_vehicle_data_notification.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/read_did_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/read_did_response.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/subscribe_vehicle_data_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/subscribe_vehicle_data_response.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/unsubscribe_vehicle_data_request.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/commands/mobile/unsubscribe_vehicle_data_response.cc"
  )

add_dynamic_library(${PROJECT_NAME})

if(BUILD_TESTS)
  add_static_library(${PROJECT_NAME})
  add_subdirectory(test)
endif()
